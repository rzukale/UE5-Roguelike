// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerUpBase.h"
#include "RLPowerUp_Credits.generated.h"

UCLASS()
class ROGUELIKE_API ARLPowerUp_Credits : public ARLPowerUpBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	ARLPowerUp_Credits();

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;

};
