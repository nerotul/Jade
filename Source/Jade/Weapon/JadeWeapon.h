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
enum class EWeaponType : uint8
{
	AR UMETA(DisplayName = "AR"),
	Sniper   UMETA(DisplayName = "Sniper"),
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

	UFUNCTION(Server, Reliable)
	void ServerTryReloadWeapon();

	UFUNCTION(BlueprintCallable)
	int GetCurrentMagazineAmmo() const { return CurrentMagazineAmmo; };

	UFUNCTION(BlueprintCallable)
	int GetMaxMagazineAmmo() const { return MaxMagazineAmmo; };

	EWeaponType GetWeaponType() const { return WeaponType; };

	void SetCurrentMagazineAmmo(int InAmmo);

	void SetBurningProjectiles(bool InIsUsingBurningProjectiles); // Runs on server

	bool GetIsUsingBurningProjectiles() const { return bIsUsingBurningProjectiles; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponType)
	EWeaponType WeaponType = EWeaponType::AR;

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

	UPROPERTY(ReplicatedUsing = OnRep_UsingBurningProjectiles)
	bool bIsUsingBurningProjectiles = false;

	UFUNCTION()
	void OnRep_UsingBurningProjectiles();

	void CreateAndApplyBurnEffect(AActor* InTargetActor, AController* InInstigatorController);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire(); // Executes on server

};
