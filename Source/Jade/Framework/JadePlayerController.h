// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Jade/Weapon/TeamInterface.h"
#include "JadePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JADE_API AJadePlayerController : public APlayerController, public ITeamInterface
{
	GENERATED_BODY()

public:	
	AJadePlayerState* ControllerPlayerState;
	
	virtual EPlayerTeam GetInstigatorTeam() override;
	
};
