// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLAction.h"
#include "RLActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API URLActionEffect : public URLAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float TimeBetweenTicks;

	FTimerHandle TimerHandle_DurationHandle;
	FTimerHandle TimerHandle_TimeBetweenTicksHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* InstigatorActor);

public:

	URLActionEffect();

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;
};
