// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "RLAction.generated.h"

class URLActionComponent;
class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ROGUELIKE_API URLAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	URLActionComponent* ActionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION(BlueprintCallable, Category = "Action")
	URLActionComponent* GetOwningComponent() const;

	UFUNCTION()
	void OnRep_RepData();

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override;

	void Initialize(URLActionComponent* NewActionComponent);
};
