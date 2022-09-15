// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ARLPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}

		return;
	}
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}

	}
}

void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ARLPlayerController::TogglePauseMenu);
}

void ARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARLPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ARLPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
}
