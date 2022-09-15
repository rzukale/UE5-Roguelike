// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLProjectileBase.generated.h"

class USoundCue;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UCameraShakeBase;

UCLASS(ABSTRACT)
class ROGUELIKE_API ARLProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLProjectileBase();

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ParticleSystemComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
};
