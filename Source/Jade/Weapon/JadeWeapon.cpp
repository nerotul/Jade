// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/JadeWeapon.h"
#include "Jade/Character/JadeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"

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

// Called every frame
void AJadeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJadeWeapon::Fire()
{
	if (!bIsReloading && !bIsFireRestrictedByRate)
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

	}
}
