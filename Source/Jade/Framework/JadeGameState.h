// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Jade/Framework/JadePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "JadeGameState.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API AJadeGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	int RedTeamScore = 0;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int BlueTeamScore = 0;

	void IncrementTeamScore(EPlayerTeam InTeamToIncrement);
};
