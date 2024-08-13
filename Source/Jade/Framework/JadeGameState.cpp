// Fill out your copyright notice in the Description page of Project Settings.


#include "Jade/Framework/JadeGameState.h"

void AJadeGameState::IncrementTeamScore(EPlayerTeam InTeamToIncrement)
{
	switch (InTeamToIncrement)
	{
	case EPlayerTeam::Red:
		RedTeamScore++;
		break;
	case EPlayerTeam::Blue:
		BlueTeamScore++;
		break;
	case EPlayerTeam::NONE:
		break;
	default:
		break;
	}
}

void AJadeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJadeGameState, RedTeamScore);
	DOREPLIFETIME(AJadeGameState, BlueTeamScore);


}
