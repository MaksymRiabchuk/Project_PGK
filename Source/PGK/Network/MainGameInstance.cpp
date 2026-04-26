#include "Network/MainGameInstance.h"

#include "Core/PGKSaveSettings.h"
#include "Kismet/GameplayStatics.h"

void UMainGameInstance::HostGame()
{
	UGameplayStatics::OpenLevel(
		GetWorld(),
		"Lvl_Default",
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

void UMainGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlot, 0))
	{
		CurrentSettings = Cast<UPGKSaveSettings>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlot, 0));
	}
	else
	{
		CurrentSettings = Cast<UPGKSaveSettings>(UGameplayStatics::CreateSaveGameObject(UPGKSaveSettings::StaticClass()));
		UGameplayStatics::SaveGameToSlot(CurrentSettings, SettingsSaveSlot, 0);
	}
}

void UMainGameInstance::SaveAudioSettings(float NewMusicVolume, float NewSFXVolume)
{
	if (CurrentSettings)
	{
		CurrentSettings->MusicVolume = NewMusicVolume;
		CurrentSettings->SFXVolume = NewSFXVolume;

		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &UMainGameInstance::OnSaveFinished);

		UE_LOG(LogTemp, Error, TEXT("CurrentSettings"));
		UGameplayStatics::AsyncSaveGameToSlot(CurrentSettings, SettingsSaveSlot, 0, SavedDelegate);
		UE_LOG(LogTemp, Error, TEXT("Word"));
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("No CurrentSettings"));
	}
}

void UMainGameInstance::OnSaveFinished(const FString& SlotName, int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved %s successfully!"), *SlotName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error while saving %s!"), *SlotName);
	}
}