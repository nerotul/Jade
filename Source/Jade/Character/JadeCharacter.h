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

	UPROPERTY(BlueprintReadOnly)
	UJadeHealthComponent* CharacterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UJadeInventoryComponent* CharacterInventory;

	UCameraComponent* GetFirstPersonCameraComponent() { return FirstPersonCameraComponent; };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Meshes and components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* ThirdPersonGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

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

	// Weapon
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AJadeWeapon> WeaponClass;

	UFUNCTION()
	void SpawnFirstPersonWeapon();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SetThirdPersonWeaponMesh)
	AJadeWeapon* CurrentWeapon = nullptr;

	UFUNCTION()
	void OnRep_SetThirdPersonWeaponMesh();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
