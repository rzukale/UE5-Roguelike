// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLInteractionComponent.generated.h"

class URLWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API URLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestInteractable();

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	UPROPERTY()
	AActor* FocusedActor;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URLWorldUserWidget> DefaultWidgetClass;
	UPROPERTY()
	URLWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();
};
