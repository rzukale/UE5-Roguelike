// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "../RogueLike.h"

const static FName SESSION_NAME = TEXT("My Session Game");

void URLGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("GameInstance initted"));

	IOnlineSubsystem* OnlineSystem = IOnlineSubsystem::Get();
	if (OnlineSystem)
	{
		UE_LOG(LogTemp, Log, TEXT("OnlineSubsytem Created %s"), *OnlineSystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("SessionInterface Created"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URLGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &URLGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URLGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URLGameInstance::OnJoinSessionComplete);
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
		}
	}
}

void URLGameInstance::RefreshServerList()
{
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void URLGameInstance::JoinGame(int32 Index)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	if (!SessionSearch.IsValid())
	{
		return;
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void URLGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccess)
{
	if (bWasSuccess == false)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Session Created"));
	GetWorld()->ServerTravel("/Game/RogueLike/Maps/ArenaLevel?listen");
}

void URLGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccess)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Destroyed successfully"));
		//CreateSession();
	}
}

void URLGameInstance::OnFindSessionsComplete(bool bWasSuccess)
{
	if (bWasSuccess == false)
	{
		return;
	}
	if (!SessionSearch.IsValid())
	{
		return;
	}
	ServerData.Empty();
	UE_LOG(LogTemp, Log, TEXT("Sessions found successfully"));
	for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Result Session ID: %s"), *Result.GetSessionIdStr());
		FServerData Data;
		Data.ServerName = Result.GetSessionIdStr();
		Data.NumberOfPlayers = Result.Session.SessionSettings.NumPublicConnections - Result.Session.NumOpenPublicConnections;
		Data.HostUserName = Result.Session.OwningUserName;
		ServerData.Emplace(Data);
	}
	UE_LOG(LogTemp, Log, TEXT("Sessions loop finished"));
	bool bFoundSessions = true;
	if (ServerData.IsEmpty())
	{
		LogOnScreen(this, "No active sessions found.");
		bFoundSessions = false;
	}
	OnSearchSessionComplete.Broadcast(bFoundSessions);
}

void URLGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success || !SessionInterface.IsValid())
	{
		return;
	}

	FString Address;
	bool bWasSuccess = SessionInterface->GetResolvedConnectString(SessionName, Address);
	if (bWasSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get resolved connection string"));
		return;
	}

	APlayerController* PC = GetFirstLocalPlayerController();
	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void URLGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings OnlineSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == NULL_SUBSYSTEM)
		{
			OnlineSettings.bIsLANMatch = true;
		}
		else
		{
			OnlineSettings.bIsLANMatch = false;
		}
		OnlineSettings.NumPublicConnections = 2;
		OnlineSettings.bShouldAdvertise = true;
		OnlineSettings.bUsesPresence = true;
		OnlineSettings.bUseLobbiesIfAvailable = true;
		SessionInterface->CreateSession(0, SESSION_NAME, OnlineSettings);
	}
}

bool URLGameInstance::DestroySession()
{
	return SessionInterface->DestroySession(SESSION_NAME);
}

void URLGameInstance::HostGame()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* NamedSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (NamedSession == nullptr)
		{
			CreateSession();
		}
		else
		{
			bool bWasDestroyed = DestroySession();
			if (bWasDestroyed == true)
			{
				CreateSession();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to Destroy active session"));
				return;
			}
		}
	}
}
