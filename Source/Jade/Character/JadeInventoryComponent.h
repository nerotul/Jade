// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Jade/Weapon/JadeWeapon.h"
#include "JadeInventoryComponent.generated.h"

class AJadeCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JADE_API UJadeInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJadeInventoryComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	TArray<TSubclassOf<AJadeWeapon>> InventoryWeapons;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventoryRifleAmmoChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInventorySniperAmmoChanged();

	UFUNCTION()
	int GetInventoryAmmo(WeaponType InWeaponType) const;

	int GetMaxInventoryAmmo(WeaponType InWeaponType) const;

	UFUNCTION()
	void SubstractInventoryAmmo(WeaponType InWeaponType, int InAmountToSubstract);

	UFUNCTION()
	void AddInventoryAmmo(WeaponType InWeaponType, int InAmmoChange);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_OnInventoryRifleAmmoChanged)
	int InventoryRifleAmmo = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxInventoryRifleAmmo = 90;

	UFUNCTION()
	void OnRep_OnInventoryRifleAmmoChanged();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_OnInventorySniperAmmoChanged)
	int InventorySniperAmmo = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxInventorySniperAmmo = 20;

	UFUNCTION()
	void OnRep_OnInventorySniperAmmoChanged();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
