// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGameModeBase.generated.h"

class ACXPlayerController;

UCLASS()
class CHATX_API ACXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumber(const FString& InNumber);

	FString JudgeResult(const FString& InSecretNumber, const FString& InGuessNumber);

	void PrintChatMessage(ACXPlayerController* InChattingPlayerController, const FString& InChatMessage);

	void IncreaseGuessCount(ACXPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ACXPlayerController* InChattingPlayerController, int32 InStrikeCount);

protected:
	FString SecretNumber;

	TArray<TObjectPtr<ACXPlayerController>> PlayerControllers;
};
