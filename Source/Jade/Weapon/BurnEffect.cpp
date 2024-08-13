// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Weapon/BurnEffect.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


void UBurnEffect::StartBurning(AActor* InTargetActor, AController* InInstigatorController, UWorld* InWorldContext)
{
	TargetActor = InTargetActor;
	InstigatorController = InInstigatorController;
	WorldContext = InWorldContext;

	if (WorldContext)
	{
		WorldContext->GetTimerManager().SetTimer(DamageTickTimer, this, &UBurnEffect::ApplyDamage, TickFrequency, true);
		WorldContext->GetTimerManager().SetTimer(EffectTimer, this, &UBurnEffect::StopBurning, EffectDuration, false);
	}
}

void UBurnEffect::StopBurning()
{
	if (WorldContext)
	{
		WorldContext->GetTimerManager().ClearTimer(DamageTickTimer);

	}
}

void UBurnEffect::ApplyDamage()
{
	UGameplayStatics::ApplyDamage(TargetActor, DamagePerTick, InstigatorController, nullptr, NULL);
}
