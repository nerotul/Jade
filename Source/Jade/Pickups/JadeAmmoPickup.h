// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JadePickup.h"
#include "Jade/Weapon/JadeWeapon.h"
#include "JadeAmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API AJadeAmmoPickup : public AJadePickup
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	WeaponType AmmoType = WeaponType::AR;

	UPROPERTY(EditAnywhere)
	int StoredAmmo = 30;
	
};
