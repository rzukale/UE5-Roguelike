// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLProjectileBase.h"
#include "RLProjectileDash.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARLProjectileDash : public ARLProjectileBase
{
	GENERATED_BODY()

public:

	ARLProjectileDash();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float ExplodeDelay;

	FTimerHandle TimerHandle_DelayedExplode;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

};
