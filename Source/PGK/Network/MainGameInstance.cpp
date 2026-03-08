#include "Network/MainGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UMainGameInstance::HostGame()
{
	UGameplayStatics::OpenLevel(
		GetWorld(),
		"Lvl_FirstPerson",
		true,
		"listen"
	);
}

void UMainGameInstance::JoinGame(FString ip)
{
	APlayerController* PC = GetFirstLocalPlayerController();

	if (PC)
	{
		PC->ClientTravel(
			ip,
			ETravelType::TRAVEL_Absolute
		);
	}
}