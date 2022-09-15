// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerState.h"
#include "RLSaveGame.h"
#include "Net/UnrealNetwork.h"

int32 ARLPlayerState::GetCredits() const
{
	return Credits;
}

void ARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta >= 0))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ARLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}
	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);
	
	return true;
}

bool ARLPlayerState::UpdateRecordTime(float NewTime)
{
	if (NewTime > RecordTime)
	{
		float OldRecord = RecordTime;
		RecordTime = NewTime;
		OnRecordTimeChanged.Broadcast(this, RecordTime, OldRecord);
		return true;
	}
	return false;
}

void ARLPlayerState::SavePlayerState_Implementation(URLSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData SaveData;
		SaveData.Credits = Credits;
		SaveData.RecordTime = RecordTime;
		SaveData.PlayerID = GetUniqueId().ToString();

		if (APawn* MyPawn = GetPawn())
		{
			SaveData.Location = MyPawn->GetActorLocation();
			SaveData.Rotation = MyPawn->GetActorRotation();
			SaveData.bResumeAtSaveLocation = true;
		}
		SaveObject->SavedPlayers.Add(SaveData);
	}
}

void ARLPlayerState::LoadPlayerState_Implementation(URLSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData* FoundData = SaveObject->GetPlayerData(this);
		if (FoundData)
		{
			AddCredits(FoundData->Credits);
			RecordTime = FoundData->RecordTime;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find savegame data for player ID '%i'"), GetPlayerId());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid SaveObject"));
	}
}

void ARLPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ARLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLPlayerState, Credits);
}