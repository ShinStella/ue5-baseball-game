// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CXChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/CXPlayerController.h"

void UCXChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput && !EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UCXChatInput::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UCXChatInput::OnChatInputTextCommitted);
	}
}

void UCXChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput && EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UCXChatInput::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UCXChatInput::OnChatInputTextCommitted);
	}
}

void UCXChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!IsValid(EditableTextBox_ChatInput))
	{
		return;
	}

	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* PlayerController = GetOwningPlayer();
		if (IsValid(PlayerController))
		{
			ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(PlayerController);
			if (IsValid(CXPlayerController))
			{
				CXPlayerController->SetChatMessage(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
