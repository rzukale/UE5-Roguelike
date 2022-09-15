// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLCharacter.generated.h"

class URLActionComponent;
class URLAttributeComponent;
class URLInteractionComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ROGUELIKE_API ARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void Dash();
	void Blackhole();
	void PrimaryInteract();
	void SprintStart();
	void SprintStop();

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URLInteractionComponent* InteractionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URLAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URLActionComponent* ActionComp;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	virtual FVector GetPawnViewLocation() const override;

};
