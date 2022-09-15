// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RLSaveGameSubsystem.generated.h"

class URLSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class URLSaveGame*, SaveObject);

UCLASS(meta=(DisplayName="SaveGame System"))
class ROGUELIKE_API URLSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	FString CurrentSlotName;

	UPROPERTY()
	URLSaveGame* CurrentSaveGame;

public:

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;

	UFUNCTION(BlueprintCallable)
	bool OverrideSpawnTransform(AController* NewPlayer);
	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString NewSlotName);
	UFUNCTION(BlueprintCallable)
	void WriteSaveGame();

	void LoadSaveGame(FString InSlotName = "");
	void HandleStartingNewPlayer(AController* NewPlayer);
	void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
