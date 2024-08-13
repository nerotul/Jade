// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Framework/JadePlayerController.h"
#include "Jade/Framework/JadePlayerState.h"


EPlayerTeam AJadePlayerController::GetInstigatorTeam()
{
	if (IsValid(ControllerPlayerState))
	{
		return ControllerPlayerState->PlayerTeam;
	}
	else
	{
		ControllerPlayerState = GetPlayerState<AJadePlayerState>();

		if (IsValid(ControllerPlayerState))
		{
			return ControllerPlayerState->PlayerTeam;
		}
		else
		{
			return EPlayerTeam::NONE;
		}
	}
}
