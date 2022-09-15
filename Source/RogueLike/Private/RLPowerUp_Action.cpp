// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUp_Action.h"
#include "RLAction.h"
#include "RLActionComponent.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

void ARLPowerUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	URLActionComponent* ActionComp = Cast<URLActionComponent>(InstigatorPawn->GetComponentByClass(URLActionComponent::StaticClass()));
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMessage = FString::Printf(TEXT("Action %s already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
			return;
		}
		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerUp();
	}
}

FText ARLPowerUp_Action::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	return LOCTEXT("PowerupAction_InteractMessage", "Grants the Sprint ability on use");
}

#undef LOCTEXT_NAMESPACE