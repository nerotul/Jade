// Fill out your copyright notice in the Description page of Project Settings.


#include "JadeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "JadeHealthComponent.h"
#include "JadeInventoryComponent.h"
#include "Jade/Weapon/JadeWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AJadeCharacter::AJadeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

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
}

void AJadeCharacter::PlayFirstPersonFireAnimation()
{
	UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(FirstPersonFireAnimation, 1.f);
	}

}

void AJadeCharacter::PlayThirdPersonFireAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThirdPersonFireAnimation, 1.f);
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
	if (WeaponClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Owner = this;

		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);

		CurrentWeapon = GetWorld()->SpawnActor<AJadeWeapon>(WeaponClass, ActorSpawnParams);

		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(FirstPersonMesh, Rule, FName("WeaponSocket"));
			OnRep_SetThirdPersonWeaponMesh();

		}
	}
}

void AJadeCharacter::OnRep_SetThirdPersonWeaponMesh()
{
	if (CurrentWeapon)
	{
		ThirdPersonGun->SetSkeletalMesh(CurrentWeapon->GetWeaponMesh());
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
		ThirdPersonGun->AttachToComponent(GetMesh(), Rule, FName("WeaponSocket"));
	}
}

void AJadeCharacter::ServerFireInputPressed_Implementation()
{
	bIsFireInputPressed = true;
}

void AJadeCharacter::ServerFireInputReleased_Implementation()
{
	bIsFireInputPressed = false;
}

void AJadeCharacter::TryToFireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
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
}

void AJadeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJadeCharacter, CharacterCameraRotation);
	DOREPLIFETIME(AJadeCharacter, CurrentWeapon);

}