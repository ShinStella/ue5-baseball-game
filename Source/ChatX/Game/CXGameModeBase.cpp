// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CXGameModeBase.h"
#include "Game/CXGameStateBase.h"
#include "Player/CXPlayerController.h"
#include "EngineUtils.h"
#include "Player/CXPlayerState.h"

void ACXGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumber = GenerateSecretNumber();
}

void ACXGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(NewPlayer);
	if (IsValid(CXPlayerController))
	{
		CXPlayerController->NotificationText = TEXT("Connected to the game server.");
		//CXPlayerController->SetNotificationText(TEXT("Connected to the game server."));

		PlayerControllers.Add(CXPlayerController);

		ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPlayerState))
		{
			CXPlayerState->PlayerName = TEXT("Player") + FString::FromInt(PlayerControllers.Num());
		}

		ACXGameStateBase* CXGameStateBase = GetGameState<ACXGameStateBase>();
		if (IsValid(CXGameStateBase))
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPlayerState->PlayerName);
		}
	}
}

FString ACXGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	UE_LOG(LogTemp, Error, TEXT("%s"), *Result);

	return Result;
}

bool ACXGameModeBase::IsGuessNumber(const FString& InNumber)
{
	bool bCanPlay = false;

	do {

		if (InNumber.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumber)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACXGameModeBase::JudgeResult(const FString& InSecretNumber, const FString& InGuessNumber)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumber[i] == InGuessNumber[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (InSecretNumber.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}

void ACXGameModeBase::PrintChatMessage(ACXPlayerController* InChattingPlayerController, const FString& InChatMessage)
{
	FString ChatMessage = InChatMessage;
	int Index = InChatMessage.Len() - 3;
	FString GuessNumber = InChatMessage.RightChop(Index);
	if (IsGuessNumber(GuessNumber))
	{
		IncreaseGuessCount(InChattingPlayerController);

		FString Result = JudgeResult(SecretNumber, GuessNumber);
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController))
			{
				FString CombinedMessage = InChatMessage + TEXT(" -> ") + Result;
				CXPlayerController->ClientRPCPrintChatMessage(CombinedMessage);

				int32 StrikeCount = FCString::Atoi(*Result.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController))
			{
				CXPlayerController->ClientRPCPrintChatMessage(InChatMessage);
			}
		}
	}
}

void ACXGameModeBase::IncreaseGuessCount(ACXPlayerController* InChattingPlayerController)
{
	ACXPlayerState* CXPlayerState = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
	if (IsValid(CXPlayerState))
	{
		CXPlayerState->CurrentGuessCount++;
	}
}

void ACXGameModeBase::ResetGame()
{
	SecretNumber = GenerateSecretNumber();

	for (const TObjectPtr<ACXPlayerController>& CXPlayerController : PlayerControllers)
	{
		ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPlayerState))
		{
			CXPlayerState->CurrentGuessCount = 0;
		}
	}
}

void ACXGameModeBase::JudgeGame(ACXPlayerController* InChattingPlayerController, int32 InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ACXPlayerState* CXPlayerState = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
		for (const TObjectPtr<ACXPlayerController>& CXPlayerController : PlayerControllers)
		{
			if (IsValid(CXPlayerState))
			{
				FString CombinedMessage = CXPlayerState->PlayerName + TEXT(" has won the game.");
				CXPlayerController->NotificationText = CombinedMessage;
				//CXPlayerController->SetNotificationText(CombinedMessage);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const TObjectPtr<ACXPlayerController>& CXPlayerController : PlayerControllers)
		{
			ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
			if (IsValid(CXPlayerState))
			{
				if (CXPlayerState->CurrentGuessCount < CXPlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const TObjectPtr<ACXPlayerController>& CXPlayerController : PlayerControllers)
			{
				CXPlayerController->NotificationText = TEXT("Draw...");
				//CXPlayerController->SetNotificationText(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
