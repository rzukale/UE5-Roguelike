// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLAICharacter.generated.h"

class URLActionComponent;
class URLWorldUserWidget;
class URLAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;

UCLASS()
class ROGUELIKE_API ARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARLAICharacter();

protected:

	URLWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URLAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URLActionComponent* ActionComp;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void OnPawnSeen(APawn* OtherPawn);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();


	virtual void PostInitializeComponents() override;
};
