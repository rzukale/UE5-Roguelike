// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLItemChest.generated.h"

UCLASS()
class ROGUELIKE_API ARLItemChest : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLItemChest();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LidMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* GoldMesh;

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

public:	
	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();
};
