// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Character/JadeInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Jade/Character/JadeCharacter.h"

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
	if (ComponentOwner)
	{
		ComponentOwner->OnInventoryRifleAmmoChanged(InventoryRifleAmmo);
	}
}

int UJadeInventoryComponent::GetInventoryAmmo(EWeaponType InWeaponType) const
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
		return InventoryRifleAmmo;
		break;
	case EWeaponType::Sniper:
		return InventorySniperAmmo;
		break;
	default:
		return 0;
		break;
	}
}

int UJadeInventoryComponent::GetMaxInventoryAmmo(EWeaponType InWeaponType) const
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
		return MaxInventoryRifleAmmo;
		break;
	case EWeaponType::Sniper:
		return MaxInventorySniperAmmo;
		break;
	default:
		return 0;
		break;
	}

}

void UJadeInventoryComponent::SubstractInventoryAmmo(EWeaponType InWeaponType, int InAmountToSubstract)
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
		InventoryRifleAmmo -= InAmountToSubstract;
		OnRep_OnInventoryRifleAmmoChanged(); // For server
		break;
	case EWeaponType::Sniper:
		InventorySniperAmmo -= InAmountToSubstract;
		OnRep_OnInventorySniperAmmoChanged(); // For server
		break;
	default:
		break;
	}

}

void UJadeInventoryComponent::AddInventoryAmmo(EWeaponType InWeaponType, int InAmmoChange)
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
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
	case EWeaponType::Sniper:
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

void UJadeInventoryComponent::UpdateStashedAmmo(EWeaponType InWeaponType, int InNewStashedAmmo)
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
		StashedRifleAmmo = InNewStashedAmmo;
		break;
	case EWeaponType::Sniper:
		StashedSniperAmmo = InNewStashedAmmo;
		break;
	default:
		break;
	}

}

int UJadeInventoryComponent::GetStashedAmmo(EWeaponType InWeaponType) const
{
	switch (InWeaponType)
	{
	case EWeaponType::AR:
		return StashedRifleAmmo;
		break;
	case EWeaponType::Sniper:
		return StashedSniperAmmo;
		break;
	default:
		return 0;
		break;
	}

}

// Called when the game starts
void UJadeInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentOwner = Cast<AJadeCharacter>(GetOwner());

	OnRep_OnInventoryRifleAmmoChanged();
	OnRep_OnInventorySniperAmmoChanged();

}


void UJadeInventoryComponent::OnRep_OnInventorySniperAmmoChanged()
{
	if (ComponentOwner)
	{
		ComponentOwner->OnInventorySniperAmmoChanged(InventorySniperAmmo);
	}
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
	DOREPLIFETIME(UJadeInventoryComponent, StashedRifleAmmo);
	DOREPLIFETIME(UJadeInventoryComponent, StashedSniperAmmo);

}