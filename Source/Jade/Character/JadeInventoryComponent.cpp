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
	default:
		return 0;
		break;
	}
}

void UJadeInventoryComponent::ChangeInventoryAmmo(WeaponType InWeaponType, int InAmmoChange)
{
	switch (InWeaponType)
	{
	case WeaponType::AR:
		InventoryRifleAmmo += InAmmoChange;
		break;
	default:
		break;
	}

	OnRep_OnInventoryRifleAmmoChanged(); // For server
}

// Called when the game starts
void UJadeInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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


}