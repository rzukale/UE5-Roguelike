// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RLAnimInstance.generated.h"

class URLActionComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API URLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	URLActionComponent* ActionComp;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;

};
