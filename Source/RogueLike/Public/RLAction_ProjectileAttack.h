// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLAction.h"
#include "RLAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
class USoundBase;

UCLASS()
class ROGUELIKE_API URLAction_ProjectileAttack : public URLAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimationDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AttackAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* CastingEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	USoundBase* CastingSound;

	UPROPERTY(EditAnywhere, Category = "Attack|Targeting")
	float SweepRadius;
	UPROPERTY(EditAnywhere, Category = "Attack|Targeting")
	float SweepDistanceFallback;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	URLAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
};
