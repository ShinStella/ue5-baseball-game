// CXGameStateBase.cpp

#include "CXGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CXPlayerController.h"

void ACXGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (!HasAuthority())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerController))
		{
			ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(PlayerController);
			if (IsValid(CXPlayerController))
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPlayerController->PrintChatMessage(NotificationString);
			}
		}
	}
}
