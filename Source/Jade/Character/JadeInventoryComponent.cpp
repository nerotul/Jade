// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Character/JadeInventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UJadeInventoryComponent::UJadeInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UJadeInventoryComponent::OnRep_OnInventoryRifleAmmoChanged()
{
	OnInventoryRifleAmmoChanged();
}

int UJadeInventoryComponent::GetInventoryAmmo(WeaponType InWeaponType) const
{
	switch (InWeaponType)
	{
	case WeaponType::AR:
		return InventoryRifleAmmo;
		break;
	case WeaponType::Sniper:
		return InventorySniperAmmo;
		break;
	default:
		return 0;
		break;
	}
}

int UJadeInventoryComponent::GetMaxInventoryAmmo(WeaponType InWeaponType) const
{
	switch (InWeaponType)
	{
	case WeaponType::AR:
		return MaxInventoryRifleAmmo;
		break;
	case WeaponType::Sniper:
		return MaxInventorySniperAmmo;
		break;
	default:
		return 0;
		break;
	}

}

void UJadeInventoryComponent::SubstractInventoryAmmo(WeaponType InWeaponType, int InAmountToSubstract)
{
	switch (InWeaponType)
	{
	case WeaponType::AR:
		InventoryRifleAmmo -= InAmountToSubstract;
		OnRep_OnInventoryRifleAmmoChanged(); // For server
		break;
	case WeaponType::Sniper:
		InventorySniperAmmo -= InAmountToSubstract;
		OnRep_OnInventorySniperAmmoChanged(); // For server
		break;
	default:
		break;
	}

}

void UJadeInventoryComponent::AddInventoryAmmo(WeaponType InWeaponType, int InAmmoChange)
{
	switch (InWeaponType)
	{
	case WeaponType::AR:
		if ((InventoryRifleAmmo + InAmmoChange) > MaxInventoryRifleAmmo)
		{
			InventoryRifleAmmo = MaxInventoryRifleAmmo;
		}
		else
		{
			InventoryRifleAmmo += InAmmoChange;
		}
		OnRep_OnInventoryRifleAmmoChanged(); // For server
		break;
	case WeaponType::Sniper:
		if ((InventorySniperAmmo + InAmmoChange) > MaxInventorySniperAmmo)
		{
			InventorySniperAmmo = MaxInventorySniperAmmo;
		}
		else
		{
			InventorySniperAmmo += InAmmoChange;
		}
		OnRep_OnInventorySniperAmmoChanged(); // For server
		break;
	default:
		break;
	}
}

// Called when the game starts
void UJadeInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UJadeInventoryComponent::OnRep_OnInventorySniperAmmoChanged()
{
	OnInventorySniperAmmoChanged();
}

// Called every frame
void UJadeInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJadeInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UJadeInventoryComponent, InventoryWeapons);
	DOREPLIFETIME(UJadeInventoryComponent, InventoryRifleAmmo);
	DOREPLIFETIME(UJadeInventoryComponent, InventorySniperAmmo);



}