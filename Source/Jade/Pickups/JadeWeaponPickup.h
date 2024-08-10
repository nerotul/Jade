// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JadePickup.h"
#include "Jade/Weapon/JadeWeapon.h"
#include "JadeWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API AJadeWeaponPickup : public AJadePickup
{
	GENERATED_BODY()
	
public:
	AJadeWeaponPickup();
	
	virtual void Interact(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable)
	void SetMagazineAmmo(int InMagazineAmmo);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AJadeWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponType)
	EWeaponType WeaponType = EWeaponType::AR;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere)
	int MagazineAmmo = 30;

};
