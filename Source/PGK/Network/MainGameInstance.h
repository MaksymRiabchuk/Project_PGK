#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class UPGKSaveSettings;
/**
 * 
 */
UCLASS()
class PGK_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void HostGame();

	UFUNCTION(BlueprintCallable)
	void JoinGame(FString ip = "127.0.0.1");
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveAudioSettings(float NewMusicVolume, float NewSFXVolume);

	UFUNCTION(BlueprintPure, Category = "Settings")
	UPGKSaveSettings* GetAudioSettings() const { return CurrentSettings; }

	// Called from the main menu to start a fresh game (no save loaded).
	UFUNCTION(BlueprintCallable, Category = "Save")
	void NewGame();

	// Called from the main menu to open the game level and trigger save restoration.
	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadSavedGame();

	// True when the game level should restore state from disk on BeginPlay.
	UPROPERTY(BlueprintReadOnly, Category = "Save")
	bool bShouldLoadSave = false;

	UFUNCTION(BlueprintPure, Category = "Save")
	bool HasSaveGame() const;

protected:
	void OnSaveFinished(const FString& SlotName, int32 UserIndex, bool bSuccess);
	UPROPERTY()
	UPGKSaveSettings* CurrentSettings;

	FString SettingsSaveSlot = TEXT("AudioSettingsSlot");
};
