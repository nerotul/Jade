// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JadePlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	Red	UMETA(DisplayName = "Red"),
	Blue   UMETA(DisplayName = "Blue"),
};

/**
 * 
 */
UCLASS()
class JADE_API AJadePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EPlayerTeam PlayerTeam;

	UPROPERTY(BlueprintReadOnly)
	int PlayerScore = 0;
	
};
