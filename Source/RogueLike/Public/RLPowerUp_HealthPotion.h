// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerUpBase.h"
#include "RLPowerUp_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARLPowerUp_HealthPotion : public ARLPowerUpBase
{
	GENERATED_BODY()

public:

	ARLPowerUp_HealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	int32 CreditCost;
};
