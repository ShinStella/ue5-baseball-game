// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CXPlayerState.h"
#include "Net/UnrealNetwork.h"

ACXPlayerState::ACXPlayerState()
	: PlayerName(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ACXPlayerState::GetPlayerInfo()
{
	FString PlayerInfo = PlayerName + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");

	return PlayerInfo;
}
