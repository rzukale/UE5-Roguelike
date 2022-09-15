// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerUpBase.h"
#include "RLPowerUp_Action.generated.h"

class URLAction;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARLPowerUp_Action : public ARLPowerUpBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<URLAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;
};
