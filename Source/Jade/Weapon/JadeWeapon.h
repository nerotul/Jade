// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JadeWeapon.generated.h"

class AJadeCharacter;
class USoundBase;
class UParticleSystem;
class AJadeProjectile;

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	AR UMETA(DisplayName = "AR"),
	Shotgun   UMETA(DisplayName = "Shotgun"),
};

UCLASS()
class JADE_API AJadeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJadeWeapon();

	USkeletalMesh* GetWeaponMesh() { return WeaponMesh->GetSkeletalMeshAsset(); };

	bool GetIsReloading() const { return bIsReloading; };

	UFUNCTION(BlueprintImplementableEvent)
	void OnMagazineAmmoChanged();

	UFUNCTION(Server, Reliable)
	void ServerTryReloadWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponType)
	WeaponType WeaponType = WeaponType::AR;

	AJadeCharacter* ThisWeaponsOwner = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxMagazineAmmo = 30;

	UPROPERTY(ReplicatedUsing = OnRep_MagazineAmmoChanged, EditDefaultsOnly, BlueprintReadOnly)
	int CurrentMagazineAmmo = MaxMagazineAmmo;

	UPROPERTY(EditDefaultsOnly)
	float WeaponDamage = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1.f;

	bool bIsFireRestrictedByRate = false;

	FTimerHandle FireRestrictionByRate;

	void SetFireRestrictedByRate();

	void SetFireAllowedByRate();

	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly)
	bool bIsUsingProjectiles = false;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TSubclassOf<AJadeProjectile> ProjectileClass;

	void FireWithTrace();

	void FireWithProjectile();

	UFUNCTION()
	void OnRep_MagazineAmmoChanged();

	// FX
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnFireFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FX)
	USoundBase* FireSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FX)
	UParticleSystem* MuzzleFire = nullptr;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastReloadFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FX)
	USoundBase* ReloadSound = nullptr;

	FTimerHandle ReloadTimerHandle;

	UFUNCTION(Server, Reliable)
	void ServerEndReloading();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire(); // Executes on server

};
