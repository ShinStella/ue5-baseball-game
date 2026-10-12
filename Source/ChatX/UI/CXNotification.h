// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CXNotification.generated.h"

class UTextBlock;

UCLASS()
class CHATX_API UCXNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNotification(FText Text);

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NotificationText;
};
