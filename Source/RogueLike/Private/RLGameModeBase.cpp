// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameModeBase.h"

#include "RLAttributeComponent.h"
#include "AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EngineUtils.h"
#include "RLActionComponent.h"
#include "RLCharacter.h"
#include "RLMonsterData.h"
#include "RLPlayerState.h"
#include "RLSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "../RogueLike.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameSession.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rl.SpawnBots"), true, TEXT("Enable Bot spawning via timer."), ECVF_Cheat);

ARLGameModeBase::ARLGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;
	CooldownBetweenFailures = 8.0f;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ARLPlayerState::StaticClass();
}

void ARLGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	URLSaveGameSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SaveSystem->LoadSaveGame(SelectedSaveSlot);
}

void ARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARLGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ARLGameModeBase::KillAll()
{
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++It)
	{
		ARLAICharacter* Bot = *It;

		URLAttributeComponent* AttribComp = URLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttribComp) && AttribComp->IsAlive())
		{
			AttribComp->Kill(this);
		}
	}
}

void ARLGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	if (SpawnCreditCurve)
	{
		AvailableSpawnCredits += SpawnCreditCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (CooldownBotSpawnUntil > GetWorld()->TimeSeconds)
	{
		return;
	}

	uint32 NumberOfBotsAlive = 0;
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++It)
	{
		ARLAICharacter* Bot = *It;

		URLAttributeComponent* AttribComp = URLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttribComp) && AttribComp->IsAlive())
		{
			NumberOfBotsAlive++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Number of Bots found: %d"), NumberOfBotsAlive);

	const float MaxBotsAllowed = 40.0f;
	if (NumberOfBotsAlive >= MaxBotsAllowed)
	{
		UE_LOG(LogTemp, Log, TEXT("Max Number of Bots, skipping spawning"));
		return;
	}

	if (MonsterTable)
	{
		SelectedMonsterRow = nullptr;

		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("", Rows);

		float TotalWeight = 0.0f;
		for (FMonsterInfoRow* Row : Rows)
		{
			TotalWeight += Row->SpawnWeight;
		}
		int32 RandomWeight = FMath::RandRange(0.0f, TotalWeight);
		TotalWeight = 0.0f;

		for (FMonsterInfoRow* Row : Rows)
		{
			TotalWeight += Row->SpawnWeight;
			if (RandomWeight <= TotalWeight)
			{
				SelectedMonsterRow = Row;
				break;
			}
		}
		if (SelectedMonsterRow && SelectedMonsterRow->SpawnWeight >= AvailableSpawnCredits)
		{
			CooldownBotSpawnUntil = GetWorld()->TimeSeconds + CooldownBetweenFailures;
			return;
		}
	}
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARLGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ARLGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0) && MonsterTable)
	{
		UAssetManager* Manager = UAssetManager::GetIfValid();
		if (Manager && SelectedMonsterRow)
		{
			//LogOnScreen(this, "Loading Monster");
			AvailableSpawnCredits -= SelectedMonsterRow->SpawnCost;
			TArray<FName> Bundles;
			FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARLGameModeBase::OnMonsterLoaded, SelectedMonsterRow->MonsterId, Locations[0]);
			Manager->LoadPrimaryAsset(SelectedMonsterRow->MonsterId, Bundles, Delegate);
		}
	}
}

void ARLGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	//LogOnScreen(this, "Finished loading Monster.");
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		URLMonsterData* MonsterData = Cast<URLMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			if (NewBot)
			{
				//LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				URLActionComponent* ActionComp = Cast<URLActionComponent>(NewBot->GetComponentByClass(URLActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<URLAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}

			}
		}
	}
}

void ARLGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup Spawn EQS Query failed"));
		return;
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	int32 SpawnIndex = 0;
	while (SpawnIndex < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector ChosenLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		bool bIsValidLocation = true;
		for (FVector Location : UsedLocations)
		{
			float DistanceTo = (ChosenLocation - Location).Size();
			if (DistanceTo < RequiredPowerupDistance)
			{
				bIsValidLocation = false;
				break;
			}
		}
		if (!bIsValidLocation)
		{
			continue;
		}
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, ChosenLocation, FRotator::ZeroRotator, Params);

		UsedLocations.Add(ChosenLocation);

		SpawnIndex++;
	}
}

void ARLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ARLGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ARLCharacter* Player = Cast<ARLCharacter>(VictimActor);
	if (Player)
	{
		ARLPlayerState* PS = Player->GetPlayerState<ARLPlayerState>();
		if (PS)
		{
			PS->UpdateRecordTime(GetWorld()->TimeSeconds);
		}
		URLSaveGameSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();
		SaveSystem->WriteSaveGame();
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn && KillerPawn != VictimActor)
	{
		ARLPlayerState* PlayerState = KillerPawn->GetPlayerState<ARLPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsPerKill);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ARLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	URLSaveGameSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();

	SaveSystem->HandleStartingNewPlayer(NewPlayer);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	SaveSystem->OverrideSpawnTransform(NewPlayer);
}


APlayerController* ARLGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	if (PC)
	{
		UE_LOG(LogTemp, Log, TEXT("Registered player"));
		GameSession->RegisterPlayer(PC, UniqueId, false);
		return PC;
	}
	return nullptr;
}

void ARLGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("Unregistering player"));
	const APlayerController* PC = Cast<APlayerController>(Exiting);
	if (PC)
	{
		GameSession->UnregisterPlayer(PC);
	}
}
