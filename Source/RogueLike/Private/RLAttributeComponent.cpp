// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAttributeComponent.h"
#include "RLGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("rl.DamageMultiplier"), 1.0f, TEXT("Global damage modifier for Attribute Component"), ECVF_Cheat);

// Sets default values for this component's properties
URLAttributeComponent::URLAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
	Rage = 0;
	MaxRage = 100;

	SetIsReplicatedByDefault(true);
}

URLAttributeComponent* URLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URLAttributeComponent>(FromActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool URLAttributeComponent::IsActorAlive(AActor* Actor)
{
	URLAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool URLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool URLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool URLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (!FMath::IsNearlyZero(ActualDelta))
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		if (ActualDelta < 0.0f && FMath::IsNearlyZero(Health))
		{
			ARLGameModeBase* GM = GetWorld()->GetAuthGameMode<ARLGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	return !FMath::IsNearlyZero(ActualDelta);
}

bool URLAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float URLAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}

float URLAttributeComponent::GetHealth() const
{
	return Health;
}

float URLAttributeComponent::GetRage() const
{
	return Rage;
}

bool URLAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - OldRage;
	if (!FMath::IsNearlyZero(ActualDelta))
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
		return true;
	}
	return false;
}

void URLAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void URLAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void URLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLAttributeComponent, Health);
	DOREPLIFETIME(URLAttributeComponent, MaxHealth);
	DOREPLIFETIME(URLAttributeComponent, Rage);
	DOREPLIFETIME(URLAttributeComponent, MaxRage);
}