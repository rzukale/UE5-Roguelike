// Fill out your copyright notice in the Description page of Project Settings.


#include "RLSaveGame.h"
#include "GameFramework/PlayerState.h"

FPlayerSaveData* URLSaveGame::GetPlayerData(APlayerState* PlayerState)
{
    if (PlayerState == nullptr)
    {
        return nullptr;
    }

    if (PlayerState->GetWorld()->IsPlayInEditor())
    {
	    // skip fetching player ID, since it won't be available in editor
        if (SavedPlayers.IsValidIndex(0))
        {
            return &SavedPlayers[0];
        }
        return nullptr;
    }

    FString PlayerID = PlayerState->GetUniqueId().ToString();
    return SavedPlayers.FindByPredicate([&](const FPlayerSaveData& Data) { return Data.PlayerID == PlayerID; });
}
