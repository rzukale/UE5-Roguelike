// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLProjectileBase.h"
#include "RLProjectileBlackhole.generated.h"


class URLActionEffect;
class URadialForceComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARLProjectileBlackhole : public ARLProjectileBase
{
	GENERATED_BODY()

public:

	ARLProjectileBlackhole();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ForceComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OuterSphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag CrushingTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<URLActionEffect> CrushingActionClass;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
