// Fill out your copyright notice in the Description page of Project Settings.


#include "JadeAmmoPickup.h"
#include "Jade/Character/JadeCharacter.h"
#include "Jade/Character/JadeInventoryComponent.h"

void AJadeAmmoPickup::Interact(AActor* Interactor)
{
	if (Interactor)
	{
		AJadeCharacter* OverlappedCharacter = Cast<AJadeCharacter>(Interactor);

		if (IsValid(OverlappedCharacter))
		{
			int InventoryAmmo = OverlappedCharacter->CharacterInventory->GetInventoryAmmo(AmmoType);
			int MaxInventoryAmmo = OverlappedCharacter->CharacterInventory->GetMaxInventoryAmmo(AmmoType);

			if (InventoryAmmo < MaxInventoryAmmo)
			{
				OverlappedCharacter->CharacterInventory->AddInventoryAmmo(AmmoType, StoredAmmo);
				Destroy();
			}

		}
	}
}
