// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUp_HealthPotion.h"
#include "RLAttributeComponent.h"
#include "RLPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ARLPowerUp_HealthPotion::ARLPowerUp_HealthPotion()
{
	CreditCost = 25;
}

void ARLPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(InstigatorPawn);
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		ARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ARLPlayerState>();
		if (PlayerState)
		{
			if (PlayerState->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerUp();
			}
		}
	}
}

FText ARLPowerUp_HealthPotion::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores to full health."), CreditCost);
}

#undef LOCTEXT_NAMESPACE