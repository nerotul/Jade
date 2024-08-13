// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Framework/JadePlayerState.h"


void AJadePlayerState::IncrementPlayerScore()
{
	PlayerScore++;
}

void AJadePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJadePlayerState, PlayerTeam);
	DOREPLIFETIME(AJadePlayerState, PlayerScore);


}
