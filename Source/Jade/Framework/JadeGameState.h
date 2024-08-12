// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JadeGameState.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API AJadeGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int RedTeamScore = 0;

	UPROPERTY(BlueprintReadOnly)
	int BlueTeamScore = 0;

	
};
