// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CXPlayerController.h"

#include "UI/CXChatInput.h"
#include "UI/CXNotification.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ChatX.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CXGameModeBase.h"
#include "CXPlayerState.h"
#include "Net/UnrealNetwork.h"

ACXPlayerController::ACXPlayerController()
{
	bReplicates = true;
}

void ACXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidget = CreateWidget<UCXChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidget))
		{
			ChatInputWidget->AddToViewport();
		}
	}

	if (IsValid(NotificationWidgetClass))
	{
		NotificationWidget = CreateWidget<UCXNotification>(this, NotificationWidgetClass);
		if (IsValid(NotificationWidget))
		{
			NotificationWidget->AddToViewport();
		}
	}
}

void ACXPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, NotificationWidget);
	DOREPLIFETIME(ThisClass, NotificationWidgetClass);
}

void ACXPlayerController::SetChatMessage(const FString& InChatMessage)
{
	ChatMessage = InChatMessage;

	if (IsLocalController())
	{
		ACXPlayerState* CXPlayerState = GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPlayerState))
		{
			FString CombinedMessage = CXPlayerState->GetPlayerInfo() + TEXT(": ") + InChatMessage;

			ServerRPCPrintChatMessage(CombinedMessage);
		}
	}
}

void ACXPlayerController::PrintChatMessage(const FString& InChatMessage)
{
	ChatXFunctionLibrary::MyPrintString(this, InChatMessage, 10.f);
}

void ACXPlayerController::SetNotificationText(const FString& InNotificationText)
{
	UE_LOG(LogTemp, Warning, TEXT("Set Notification Text"));
	NotificationText = InNotificationText;

	if (NotificationWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Notification Widget Exist"));
		NotificationWidget->SetNotification(FText::FromString(NotificationText));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Notification Widget Not Exist"));
	}
}

void ACXPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	PrintChatMessage(InChatMessage);
}

void ACXPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	/*for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
	{
		ACXPlayerController* CXPlayerController = *It;
		if (IsValid(CXPlayerController))
		{
			CXPlayerController->ClientRPCPrintChatMessage(InChatMessage);
		}
	}*/

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode))
	{
		ACXGameModeBase* CXGameMode = Cast<ACXGameModeBase>(GameMode);
		if (IsValid(CXGameMode))
		{
			CXGameMode->PrintChatMessage(this, InChatMessage);
		}
	}
}
