// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

class URLSaveGame;
class ARLPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ARLPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecordTimeChanged, ARLPlayerState*, PlayerState, float, NewTime, float, OldTime);

UCLASS()
class ROGUELIKE_API ARLPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "Credits")
	int32 Credits;
	UPROPERTY(BlueprintReadOnly)
	float RecordTime;
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:

	UFUNCTION(BlueprintCallable)
	bool UpdateRecordTime(float NewTime);
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);
	UFUNCTION(BlueprintNativeEvent, Category = "SaveGame")
	void SavePlayerState(URLSaveGame* SaveObject);
	UFUNCTION(BlueprintNativeEvent, Category = "SaveGame")
	void LoadPlayerState(URLSaveGame* SaveObject);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRecordTimeChanged OnRecordTimeChanged;
};
