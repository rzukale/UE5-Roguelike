// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URLAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnRageChanged;
};
