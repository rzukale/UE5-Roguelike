// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "RLGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchSessionComplete, bool, bFoundSessions);

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	UPROPERTY(BlueprintReadOnly)
	int32 NumberOfPlayers;
	UPROPERTY(BlueprintReadOnly)
	FString HostUserName;
};

UCLASS()
class ROGUELIKE_API URLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void RefreshServerList();

	UFUNCTION(BlueprintCallable)
	void JoinGame(int32 Index);

protected:

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccess);
	void OnFindSessionsComplete(bool bWasSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();

	UFUNCTION(BlueprintCallable)
	bool DestroySession();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	UFUNCTION(BlueprintCallable)
	void HostGame();

	UPROPERTY(BlueprintAssignable)
	FOnSearchSessionComplete OnSearchSessionComplete;

	UPROPERTY(BlueprintReadOnly)
	TArray<FServerData> ServerData;

};
