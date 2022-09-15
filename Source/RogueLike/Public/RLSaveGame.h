// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RLSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName ActorName;
	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString PlayerID;
	UPROPERTY()
	int32 Credits;
	UPROPERTY()
	float RecordTime;
	UPROPERTY()
	FVector Location;
	UPROPERTY()
	FRotator Rotation;
	UPROPERTY()
	bool bResumeAtSaveLocation;
};

UCLASS()
class ROGUELIKE_API URLSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
};
