// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CXNotification.h"
#include "Components/TextBlock.h"

void UCXNotification::SetNotification(FText Text)
{
	if (NotificationText)
	{
		NotificationText->SetText(Text);
	}
}
