// Fill out your copyright notice in the Description page of Project Settings.


#include "JadeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "JadeHealthComponent.h"
#include "JadeInventoryComponent.h"
#include "Jade/Weapon/JadeWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Jade/Pickups/PickupInterface.h"
#include "Jade/Weapon/TeamInterface.h"


// Sets default values
AJadeCharacter::AJadeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AJadeCharacter::OnBeginOverlap);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterFirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	ThirdPersonGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonGun"));
	ThirdPersonGun->SetOnlyOwnerSee(false);
	ThirdPersonGun->bCastDynamicShadow = false;
	ThirdPersonGun->CastShadow = false;

	CharacterHealth = CreateDefaultSubobject<UJadeHealthComponent>(TEXT("CharacterHealthComponent"));
	CharacterInventory = CreateDefaultSubobject<UJadeInventoryComponent>(TEXT("CharacterInventoryComponent"));

	CharacterHealth->OnCharacterDead.AddUniqueDynamic(this, &AJadeCharacter::KillCharacter);

}

void AJadeCharacter::PlayFirstPersonFireAnimation()
{
	UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(FirstPersonFireAnimation, 1.f);
	}

}

void AJadeCharacter::PlayThirdPersonFireAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(ThirdPersonFireAnimation, 1.f);
	}

}

void AJadeCharacter::PlayFirstPersonReloadAnimation()
{
	UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(FirstPersonReloadAnimation, 1.5f);
	}

}

void AJadeCharacter::PlayThirdPersonReloadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(ThirdPersonReloadAnimation, 1.5f);
	}

}

EPlayerTeam AJadeCharacter::GetCharacterTeam()
{
	if (IsValid(CharacterPlayerState))
	{
		return CharacterPlayerState->PlayerTeam;
	}
	else
	{
		CharacterPlayerState = GetPlayerState<AJadePlayerState>();

		if (IsValid(CharacterPlayerState))
		{
			return CharacterPlayerState->PlayerTeam;
		}
		else
		{
			return EPlayerTeam::NONE;
		}
	}
}

// Called when the game starts or when spawned
void AJadeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnFirstPersonWeapon();
	}

}

void AJadeCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Interface = Cast<IPickupInterface>(OtherActor);
	if (Interface)
	{
		Interface->Interact(this);
	}
}

void AJadeCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AJadeCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AJadeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJadeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	ServerSyncCameraPitch(FirstPersonCameraComponent->GetComponentRotation().Pitch);

}

void AJadeCharacter::ServerSyncCameraPitch_Implementation(float InPitch)
{
	FRotator NewRotation = FirstPersonCameraComponent->GetComponentRotation();
	NewRotation.Pitch = InPitch;
	CharacterCameraRotation = NewRotation;
	FirstPersonCameraComponent->SetWorldRotation(NewRotation);
}

void AJadeCharacter::SpawnFirstPersonWeapon()
{
	if (CharacterInventory->InventoryWeapons.IsValidIndex(CurrentWeaponInventoryIndex))
	{
		TSubclassOf<AJadeWeapon> WeaponClass = CharacterInventory->InventoryWeapons[CurrentWeaponInventoryIndex];
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = this;

		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);

		CurrentWeapon = GetWorld()->SpawnActor<AJadeWeapon>(WeaponClass, ActorSpawnParams);
		CurrentWeapon->SetCurrentMagazineAmmo(CharacterInventory->GetStashedAmmo(CurrentWeapon->GetWeaponType()));

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->AttachToComponent(FirstPersonMesh, Rule, FName("WeaponSocket"));
			OnRep_SetThirdPersonWeaponMesh();

		}
	}
}

void AJadeCharacter::OnRep_SetThirdPersonWeaponMesh()
{
	if (IsValid(CurrentWeapon))
	{
		ThirdPersonGun->SetSkeletalMesh(CurrentWeapon->GetWeaponMesh());
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
		ThirdPersonGun->AttachToComponent(GetMesh(), Rule, FName("WeaponSocket"));
		OnWeaponChanged();
	}
}

void AJadeCharacter::ServerFireInputPressed_Implementation()
{
	if (bIsAlive)
	{
		bIsFireInputPressed = true;
	}
}

void AJadeCharacter::ServerFireInputReleased_Implementation()
{
	if (bIsAlive)
	{
		bIsFireInputPressed = false;
	}
}

void AJadeCharacter::TryToFireWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Fire();
	}
}

float AJadeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsAlive)
	{
		ITeamInterface* TeamInterface = Cast<ITeamInterface>(EventInstigator);
		if (TeamInterface)
		{
			if (TeamInterface->GetInstigatorTeam() != GetCharacterTeam())
			{
				float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
				LastDamageInstigator = EventInstigator;
				CharacterHealth->ChangeHealthValue(-DamageAmount);
				return ActualDamage;
			}
		}

		return 0;

	}
	else
	{
		return 0;
	}
}

void AJadeCharacter::ServerSwitchToNextWeapon_Implementation()
{
	if (bIsAlive && IsValid(CharacterInventory) && !CurrentWeapon->GetIsReloading())
	{
		if (CharacterInventory->InventoryWeapons.IsValidIndex(CurrentWeaponInventoryIndex + 1))
		{
			CharacterInventory->UpdateStashedAmmo(CurrentWeapon->GetWeaponType(), CurrentWeapon->GetCurrentMagazineAmmo());
			CurrentWeapon->Destroy();
			CurrentWeaponInventoryIndex += 1;
			SpawnFirstPersonWeapon();

		}
		else
		{
			CharacterInventory->UpdateStashedAmmo(CurrentWeapon->GetWeaponType(), CurrentWeapon->GetCurrentMagazineAmmo());
			CurrentWeapon->Destroy();
			CurrentWeaponInventoryIndex = 0;
			SpawnFirstPersonWeapon();

		}
	}
}

void AJadeCharacter::ServerSwitchToPreviousWeapon_Implementation()
{
	if (bIsAlive && IsValid(CharacterInventory) && !CurrentWeapon->GetIsReloading())
	{
		if (CharacterInventory->InventoryWeapons.IsValidIndex(CurrentWeaponInventoryIndex - 1))
		{
			CharacterInventory->UpdateStashedAmmo(CurrentWeapon->GetWeaponType(), CurrentWeapon->GetCurrentMagazineAmmo());
			CurrentWeapon->Destroy();
			CurrentWeaponInventoryIndex -= 1;
			SpawnFirstPersonWeapon();

		}
		else
		{
			CharacterInventory->UpdateStashedAmmo(CurrentWeapon->GetWeaponType(), CurrentWeapon->GetCurrentMagazineAmmo());
			CurrentWeapon->Destroy();
			CurrentWeaponInventoryIndex = (CharacterInventory->InventoryWeapons.Num() - 1);
			SpawnFirstPersonWeapon();

		}

	}
}

void AJadeCharacter::ServerReloadWeapon_Implementation()
{
	if (bIsAlive && IsValid(CurrentWeapon))
	{
		CurrentWeapon->ServerTryReloadWeapon();
	}
}

void AJadeCharacter::ServerTryDropWeapon_Implementation()
{
	if (bIsAlive && !CurrentWeapon->GetIsReloading() && CharacterInventory->InventoryWeapons.Num() >= 2)
	{
		OnDropWeapon(CurrentWeapon); // Will spawn new weapon in blueprint
		CharacterInventory->InventoryWeapons.Remove(CurrentWeapon->GetClass());
		ServerSwitchToNextWeapon();
	}
}

void AJadeCharacter::KillCharacter()
{
	if (HasAuthority())
	{
		bIsAlive = false;
		OnRep_CharacterIsDead(); // For server
	}
}

void AJadeCharacter::OnRep_CharacterIsDead()
{
	USkeletalMeshComponent* ThirdPersonMesh = ACharacter::GetMesh();

	RootComponent = FirstPersonCameraComponent;
	GetCapsuleComponent()->DestroyComponent();
	ThirdPersonMesh->SetSimulatePhysics(true);
	ThirdPersonMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CurrentWeapon->Destroy();
	FirstPersonMesh->DestroyComponent();
	GetWorldTimerManager().SetTimer(DestroyActorHandle, this, &AJadeCharacter::DestroyPawn, BodyDisappearanceDelay, false);
	OnCharacterDead();

}

void AJadeCharacter::DestroyPawn()
{
	Destroy();
}

void AJadeCharacter::ServerToggleBurningProjectiles_Implementation()
{
	if (IsValid(CurrentWeapon))
	{
		if (CurrentWeapon->GetIsUsingBurningProjectiles())
		{
			CurrentWeapon->SetBurningProjectiles(false);
		}
		else
		{
			CurrentWeapon->SetBurningProjectiles(true);
		}
	}
}

// Called every frame
void AJadeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFireInputPressed)
	{
		TryToFireWeapon();
	}

}

// Called to bind functionality to input
void AJadeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AJadeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AJadeCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AJadeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AJadeCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJadeCharacter::ServerFireInputPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AJadeCharacter::ServerFireInputReleased);

	PlayerInputComponent->BindAction("SwitchToNextWeapon", IE_Pressed, this, &AJadeCharacter::ServerSwitchToNextWeapon);
	PlayerInputComponent->BindAction("SwitchToPreviousWeapon", IE_Pressed, this, &AJadeCharacter::ServerSwitchToPreviousWeapon);

	PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, this, &AJadeCharacter::ServerReloadWeapon);

	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AJadeCharacter::ServerTryDropWeapon);

	PlayerInputComponent->BindAction("ToggleBurningProjectiles", IE_Pressed, this, &AJadeCharacter::ServerToggleBurningProjectiles);

}

void AJadeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJadeCharacter, CharacterCameraRotation);
	DOREPLIFETIME(AJadeCharacter, CurrentWeapon);
	DOREPLIFETIME(AJadeCharacter, bIsAlive);

}