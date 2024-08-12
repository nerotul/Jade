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

	UFUNCTION()
	int GetInventoryAmmo(EWeaponType InWeaponType) const;

	int GetMaxInventoryAmmo(EWeaponType InWeaponType) const;

	UFUNCTION()
	void SubstractInventoryAmmo(EWeaponType InWeaponType, int InAmountToSubstract);

	UFUNCTION()
	void AddInventoryAmmo(EWeaponType InWeaponType, int InAmmoChange);

	UFUNCTION()
	void UpdateStashedAmmo(EWeaponType InWeaponType, int InNewStashedAmmo);

	UFUNCTION()
	int GetStashedAmmo(EWeaponType InWeaponType) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AJadeCharacter* ComponentOwner = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_OnInventoryRifleAmmoChanged)
	int InventoryRifleAmmo = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxInventoryRifleAmmo = 90;
	UPROPERTY(BlueprintReadOnly, Replicated)
	int StashedRifleAmmo = 30;
	UFUNCTION()
	void OnRep_OnInventoryRifleAmmoChanged();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_OnInventorySniperAmmoChanged)
	int InventorySniperAmmo = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxInventorySniperAmmo = 20;
	UPROPERTY(BlueprintReadOnly, Replicated)
	int StashedSniperAmmo = 5;
	UFUNCTION()
	void OnRep_OnInventorySniperAmmoChanged();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
