// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JadeWeapon.generated.h"

class AJadeCharacter;
class USoundBase;
class UParticleSystem;

UCLASS()
class JADE_API AJadeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJadeWeapon();

	USkeletalMesh* GetWeaponMesh() { return WeaponMesh->GetSkeletalMeshAsset(); };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	AJadeCharacter* ThisWeaponsOwner = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float WeaponDamage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1.f;

	bool bIsFireRestrictedByRate = false;

	FTimerHandle FireRestrictionByRate;

	void SetFireRestrictedByRate();

	void SetFireAllowedByRate();

	bool bIsReloading = false;

	bool bIsUsingProjectiles = false;

	void FireWithTrace();

	void FireWithProjectile();

	// FX
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnFireFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FX)
	USoundBase* FireSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FX)
	UParticleSystem* MuzzleFire = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

};
