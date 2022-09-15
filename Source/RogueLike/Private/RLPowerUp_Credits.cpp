// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUp_Credits.h"
#include "RLPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ARLPowerUp_Credits::ARLPowerUp_Credits()
{
	CreditsAmount = 50;
}

void ARLPowerUp_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ARLPlayerState>())
	{
		PlayerState->AddCredits(CreditsAmount);
		HideAndCooldownPowerUp();
	}
}

FText ARLPowerUp_Credits::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	return FText::Format(LOCTEXT("Credit_InteractMessage", "Grants {0} credits on use"), CreditsAmount);
}

#undef LOCTEXT_NAMESPACE