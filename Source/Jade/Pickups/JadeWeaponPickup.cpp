// Fill out your copyright notice in the Description page of Project Settings.


#include "JadeWeaponPickup.h"
#include "Jade/Character/JadeCharacter.h"
#include "Jade/Character/JadeInventoryComponent.h"


AJadeWeaponPickup::AJadeWeaponPickup()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

}

void AJadeWeaponPickup::Interact(AActor* Interactor)
{
	if (Interactor)
	{
		AJadeCharacter* OverlappedCharacter = Cast<AJadeCharacter>(Interactor);

		if (IsValid(OverlappedCharacter))
		{
			if (OverlappedCharacter->CharacterInventory->InventoryWeapons.Find(WeaponClass) == INDEX_NONE)
			{
				OverlappedCharacter->CharacterInventory->InventoryWeapons.Add(WeaponClass);
				OverlappedCharacter->CharacterInventory->UpdateStashedAmmo(WeaponType, MagazineAmmo);
				Destroy();
			}
		}
	}

}

void AJadeWeaponPickup::SetMagazineAmmo(int InMagazineAmmo)
{
	MagazineAmmo = InMagazineAmmo;
}
