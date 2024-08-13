// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BurnEffect.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API UBurnEffect : public UObject
{
	GENERATED_BODY()

public:
	void StartBurning(AActor* InTargetActor, AController* InInstigatorController, UWorld* InWorldContext);

	void StopBurning();

protected:
	void ApplyDamage();

	FTimerHandle DamageTickTimer;

	FTimerHandle EffectTimer;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	AController* InstigatorController;

	UPROPERTY()
	UWorld* WorldContext;

	float DamagePerTick = 10.f;

	float TickFrequency = 1.f;

	float EffectDuration = 4.f;
};
