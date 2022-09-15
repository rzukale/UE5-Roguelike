// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLActionEffect.h"
#include "RLActionEffect_Thorns.generated.h"

class URLAttributeComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API URLActionEffect_Thorns : public URLActionEffect
{
	GENERATED_BODY()

public:

	URLActionEffect_Thorns();

	void StartAction_Implementation(AActor* InstigatorActor) override;
	void StopAction_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);
};
