// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CXPlayerController.generated.h"

class UCXChatInput;
class UCXNotification;

UCLASS()
class CHATX_API ACXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACXPlayerController();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessage(const FString& InChatMessage);
	void PrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InChatMessage);

	void SetNotificationText(const FString& InNotificationText);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<UCXChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCXChatInput> ChatInputWidget;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<UCXNotification> NotificationWidgetClass;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<UCXNotification> NotificationWidget;

public:
	FString ChatMessage;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString NotificationText;
};
