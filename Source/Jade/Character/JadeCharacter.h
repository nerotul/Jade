// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JadeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UJadeHealthComponent;
class UJadeInventoryComponent;
class AJadeWeapon;

UCLASS()
class JADE_API AJadeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJadeCharacter();

	// Meshes and components
	UPROPERTY(BlueprintReadOnly)
	UJadeHealthComponent* CharacterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UJadeInventoryComponent* CharacterInventory;

	UCameraComponent* GetFirstPersonCameraComponent() { return FirstPersonCameraComponent; };



	// Weapon and damage
	void PlayFirstPersonFireAnimation();

	void PlayThirdPersonFireAnimation();

	USkeletalMeshComponent* GetThirdPersonGun() { return ThirdPersonGun; };

	void PlayFirstPersonReloadAnimation();

	void PlayThirdPersonReloadAnimation();

	float GetThirdPersonReloadAnimLength() { return ThirdPersonReloadAnimation->GetPlayLength(); };

	UPROPERTY(BlueprintReadOnly)
	AController* LastDamageInstigator;

	// Widget updates
	UFUNCTION(BlueprintImplementableEvent)
	void OnArmorChanged(float NewArmor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMagazineAmmoChanged(float NewMagazineAmmo);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventoryRifleAmmoChanged(float NewAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventorySniperAmmoChanged(float NewAmount);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Meshes and components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMesh; // Using default mesh as third person

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* ThirdPersonGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Controls and movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UFUNCTION(Server, Unreliable)
	void ServerSyncCameraPitch(float InPitch);

	UPROPERTY(BlueprintReadOnly, Replicated)
	FRotator CharacterCameraRotation;

	// Weapon and damage
	UFUNCTION()
	void SpawnFirstPersonWeapon(); // Called only on server

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SetThirdPersonWeaponMesh)
	AJadeWeapon* CurrentWeapon = nullptr;

	UFUNCTION()
	void OnRep_SetThirdPersonWeaponMesh();

	bool bIsFireInputPressed = false;

	UFUNCTION(Server, Unreliable)
	void ServerFireInputPressed();

	UFUNCTION(Server, Unreliable)
	void ServerFireInputReleased();

	UFUNCTION()
	void TryToFireWeapon(); // Called only on server

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* FirstPersonFireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	UAnimMontage* ThirdPersonFireAnimation;

	int32 CurrentWeaponInventoryIndex = 0;

	UFUNCTION(Server, Reliable)
	void ServerSwitchToNextWeapon();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToPreviousWeapon();

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* FirstPersonReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* ThirdPersonReloadAnimation;

	UFUNCTION(Server, Reliable)
	void ServerReloadWeapon();

	UFUNCTION(Server, Reliable)
	void ServerTryDropWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDropWeapon(AJadeWeapon* WeaponDropped);

	bool bIsAlive = true;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
