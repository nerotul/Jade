// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/JadeWeapon.h"
#include "Jade/Character/JadeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Jade/Character/JadeInventoryComponent.h"
#include "Jade/Weapon/JadeProjectile.h"


// Sets default values
AJadeWeapon::AJadeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterWeapon"));
	WeaponMesh->SetOnlyOwnerSee(true);
	RootComponent = WeaponMesh;
}

void AJadeWeapon::SetCurrentMagazineAmmo(int InAmmo)
{
	CurrentMagazineAmmo = InAmmo;
}

// Called when the game starts or when spawned
void AJadeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(FireRestrictionByRate, this, &AJadeWeapon::SetFireAllowedByRate, FireRate, true);

	ThisWeaponsOwner = Cast<AJadeCharacter>(GetOwner());
}

void AJadeWeapon::SetFireRestrictedByRate()
{
	bIsFireRestrictedByRate = true;
}

void AJadeWeapon::SetFireAllowedByRate()
{
	bIsFireRestrictedByRate = false;
}

void AJadeWeapon::FireWithTrace()
{
	FVector StartLocation = ThisWeaponsOwner->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector EndLocation;
	EndLocation = StartLocation + (ThisWeaponsOwner->GetFirstPersonCameraComponent()->GetForwardVector() * 10000.0f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ThisWeaponsOwner);
	FHitResult Hit;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation,
		ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

	UGameplayStatics::ApplyDamage(Hit.GetActor(), WeaponDamage, ThisWeaponsOwner->GetInstigatorController(), ThisWeaponsOwner, NULL);

}

void AJadeWeapon::FireWithProjectile()
{
	if (ProjectileClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			const FRotator SpawnRotation = ThisWeaponsOwner->GetFirstPersonCameraComponent()->GetComponentRotation();
			const USkeletalMeshSocket* Socket = WeaponMesh->GetSocketByName("MuzzleFlash");
			const FVector SpawnLocation = Socket->GetSocketLocation(WeaponMesh);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			AJadeProjectile* Projectile = World->SpawnActor<AJadeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (Projectile && ThisWeaponsOwner)
			{
				Projectile->SetProjectileDamage(WeaponDamage);
				Projectile->SetInstigatorController(ThisWeaponsOwner->GetInstigatorController());
			}
		}

	}
}

void AJadeWeapon::OnRep_MagazineAmmoChanged()
{
	if (ThisWeaponsOwner)
	{
		ThisWeaponsOwner->OnMagazineAmmoChanged(CurrentMagazineAmmo);
	}
}

void AJadeWeapon::ServerTryReloadWeapon_Implementation()
{
	if (!bIsReloading && CurrentMagazineAmmo != MaxMagazineAmmo)
	{
		int MagazineEmptySpace = MaxMagazineAmmo - CurrentMagazineAmmo;
		int InventoryAmmo = ThisWeaponsOwner->CharacterInventory->GetInventoryAmmo(WeaponType);

		if (MagazineEmptySpace > 0 && InventoryAmmo != 0)
		{
			bIsReloading = true;

			if (InventoryAmmo > MagazineEmptySpace)
			{
				CurrentMagazineAmmo += MagazineEmptySpace;
				ThisWeaponsOwner->CharacterInventory->SubstractInventoryAmmo(WeaponType, MagazineEmptySpace);

			}
			else
			{
				CurrentMagazineAmmo += InventoryAmmo;
				ThisWeaponsOwner->CharacterInventory->SubstractInventoryAmmo(WeaponType, InventoryAmmo);

			}

			OnRep_MagazineAmmoChanged();
			MulticastReloadFX();

		}


	}
}

void AJadeWeapon::MulticastOnFireFX_Implementation()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}

	if (ThisWeaponsOwner)
	{
		ThisWeaponsOwner->PlayFirstPersonFireAnimation();
		ThisWeaponsOwner->PlayThirdPersonFireAnimation();
	}

	if (MuzzleFire)
	{
		const USkeletalMeshSocket* Socket = WeaponMesh->GetSocketByName("MuzzleFlash");
		FTransform Transform = Socket->GetSocketTransform(WeaponMesh);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFire, Transform);

	}

}

void AJadeWeapon::MulticastReloadFX_Implementation()
{
	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
	}

	if (ThisWeaponsOwner)
	{
		ThisWeaponsOwner->PlayFirstPersonReloadAnimation();
		ThisWeaponsOwner->PlayThirdPersonReloadAnimation();
		float AnimLength = ThisWeaponsOwner->GetThirdPersonReloadAnimLength();
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AJadeWeapon::ServerEndReloading, AnimLength, false);
	}

}

void AJadeWeapon::ServerEndReloading_Implementation()
{
	bIsReloading = false;
}

// Called every frame
void AJadeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJadeWeapon::Fire()
{
	if (CurrentMagazineAmmo > 0 && !bIsReloading && !bIsFireRestrictedByRate)
	{
		SetFireRestrictedByRate();

		if (bIsUsingProjectiles)
		{
			FireWithProjectile();
		}
		else
		{
			FireWithTrace();
		}

		MulticastOnFireFX();

		CurrentMagazineAmmo -= 1;
		OnRep_MagazineAmmoChanged(); // For server
	}
	else if(CurrentMagazineAmmo == 0)
	{
		ServerTryReloadWeapon();
	}
}

void AJadeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJadeWeapon, CurrentMagazineAmmo);

}