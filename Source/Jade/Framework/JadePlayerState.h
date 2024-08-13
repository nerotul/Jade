// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "JadePlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	Red	UMETA(DisplayName = "Red"),
	Blue   UMETA(DisplayName = "Blue"),
	NONE,
};

/**
 * 
 */
UCLASS()
class JADE_API AJadePlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	EPlayerTeam PlayerTeam;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int PlayerScore = 0;
	
};
