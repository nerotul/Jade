// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Character/JadeHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UJadeHealthComponent::UJadeHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UJadeHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UJadeHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UJadeHealthComponent::ChangeArmorValue(float ChangeValue)
{
	bool bIsDamage = (ChangeValue < 0);

	if (bIsDamage)
	{
		if ((GetCurrentArmor() + ChangeValue) < 0)
		{
			CurrentArmor += ChangeValue;
			ChangeHealthValue(CurrentArmor);
			CurrentArmor = 0.0f;

		}
		else
		{
			CurrentArmor += ChangeValue;

		}
	}
	else
	{
		(GetCurrentArmor() + ChangeValue) > MaxArmor ? CurrentArmor = MaxArmor : CurrentArmor += ChangeValue;
	}
}

void UJadeHealthComponent::ChangeHealthValue(float ChangeValue)
{
	bool bIsDamage = (ChangeValue < 0);

	if (bIsDamage)
	{
		if (GetCurrentArmor() > 0)
		{
			ChangeArmorValue(ChangeValue);
		}
		else
		{
			CurrentHealth += ChangeValue;
		}

		if (CurrentHealth <= 0)
		{
			OnCharacterDead.Broadcast();
		}
	}
	else
	{
		(GetCurrentHealth() + ChangeValue) > MaxHealth ? CurrentHealth = MaxHealth : CurrentHealth += ChangeValue;

	}

}

void UJadeHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UJadeHealthComponent, CurrentHealth);
	DOREPLIFETIME(UJadeHealthComponent, CurrentArmor);

}
