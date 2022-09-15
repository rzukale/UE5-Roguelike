// Fill out your copyright notice in the Description page of Project Settings.


#include "RLSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "RLGameplayInterface.h"
#include "RLPlayerState.h"
#include "RLSaveGame.h"
#include "RLSaveGameSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void URLSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const URLSaveGameSettings* SaveSystemSettings = GetDefault<URLSaveGameSettings>();
    CurrentSlotName = SaveSystemSettings->SaveSlotName;

    // example of how to fetch soft referenced data
    //UDataTable* DummyTable = SaveSystemSettings->DummyDataTablePath.LoadSynchronous();
}


bool URLSaveGameSubsystem::OverrideSpawnTransform(AController* NewPlayer)
{
    if (NewPlayer == nullptr || !IsValid(NewPlayer))
    {
        return false;
    }
    ARLPlayerState* PS = NewPlayer->GetPlayerState<ARLPlayerState>();
    if (PS == nullptr)
    {
        return false;
    }

    APawn* MyPawn = PS->GetPawn();
    if (MyPawn)
    {
        FPlayerSaveData* FoundData = CurrentSaveGame->GetPlayerData(PS);
        if (FoundData && FoundData->bResumeAtSaveLocation)
        {
            MyPawn->SetActorLocation(FoundData->Location);
            MyPawn->SetActorRotation(FoundData->Rotation);

            AController* PlayerController = Cast<AController>(PS->GetOwner());
            if (PlayerController)
            {
                PlayerController->SetControlRotation(FoundData->Rotation);
            }
            return true;
        }
    }
    return false;
}

void URLSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
    if (NewSlotName.Len() == 0)
    {
        return;
    }
    CurrentSlotName = NewSlotName;
}

void URLSaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedActors.Empty();
	CurrentSaveGame->SavedPlayers.Empty();

    AGameStateBase* GameState = GetWorld()->GetGameState();
    if (GameState == nullptr)
    {
        return;
    }

    // iterate through player states, we don't have proper ID to match since it would require Steam/EOS
    for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
    {
        ARLPlayerState* PlayerState = Cast<ARLPlayerState>(GameState->PlayerArray[i]);
        if (PlayerState)
        {
            PlayerState->SavePlayerState(CurrentSaveGame);
            break; // single player only for now
        }
    }

    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;

        if (!IsValid(Actor) || !Actor->Implements<URLGameplayInterface>())
        {
            continue;
        }
        FActorSaveData ActorData;
        ActorData.ActorName = Actor->GetFName();
        ActorData.Transform = Actor->GetActorTransform();

        FMemoryWriter MemWriter(ActorData.ByteData);

        FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
        Ar.ArIsSaveGame = true;
        Actor->Serialize(Ar);

        CurrentSaveGame->SavedActors.Add(ActorData);
    }

    UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

    OnSaveGameWritten.Broadcast(CurrentSaveGame);

}

void URLSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
    SetSlotName(InSlotName);

    if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
    {
        CurrentSaveGame = Cast<URLSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
        if (CurrentSaveGame == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load save game data."));
            return;
        }
        UE_LOG(LogTemp, Log, TEXT("Savegame Data loaded."));

        for (FActorIterator It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;

            if (!Actor->Implements<URLGameplayInterface>())
            {
                continue;
            }

            for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
            {
	            if (ActorData.ActorName == Actor->GetFName())
	            {
                    Actor->SetActorTransform(ActorData.Transform);

                    FMemoryReader MemReader(ActorData.ByteData);

                    FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
                    Ar.ArIsSaveGame = true;
                    Actor->Serialize(Ar);

                    IRLGameplayInterface::Execute_OnActorLoaded(Actor);
                    break;
	            }
            }
        }
        OnSaveGameLoaded.Broadcast(CurrentSaveGame);
    }
    else
    {
        CurrentSaveGame = Cast<URLSaveGame>(UGameplayStatics::CreateSaveGameObject(URLSaveGame::StaticClass()));

        UE_LOG(LogTemp, Log, TEXT("New Savegame Created"));
    }

}

void URLSaveGameSubsystem::HandleStartingNewPlayer(AController* NewPlayer)
{
    ARLPlayerState* PS = NewPlayer->GetPlayerState<ARLPlayerState>();
    if (ensure(PS))
    {
        PS->LoadPlayerState(CurrentSaveGame);
    }
}


