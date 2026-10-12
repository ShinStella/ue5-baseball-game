// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerState.generated.h"

UCLASS()
class CHATX_API ACXPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACXPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfo();

public:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
