// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLPowerUpBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ROGUELIKE_API ARLPowerUpBase : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLPowerUpBase();

protected:

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive");
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "PowerUp Modifiers")
	float RespawnTime;

	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION()
	void ShowPowerUp();

	void SetPowerUpState(bool bIsActive);

	void HideAndCooldownPowerUp();

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractionText_Implementation(APawn* InstigatorPawn);

};
