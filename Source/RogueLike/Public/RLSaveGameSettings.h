// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RLSaveGameSettings.generated.h"

class UDataTable;

UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Save Game Settings"))
class ROGUELIKE_API URLSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	URLSaveGameSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
		FString SaveSlotName;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> DummyDataTablePath;

};
