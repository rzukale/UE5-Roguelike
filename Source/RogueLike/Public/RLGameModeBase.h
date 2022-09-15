// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RLGameModeBase.generated.h"

class UDataTable;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		SpawnWeight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};


UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARLGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:

	float AvailableSpawnCredits;
	float CooldownBotSpawnUntil;

	FMonsterInfoRow* SelectedMonsterRow;
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* SpawnCreditCurve;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float CooldownBetweenFailures;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* PowerupSpawnQuery;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

};
