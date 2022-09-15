// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ARLAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(AIBehavior, TEXT("Behaviortree is nullptr. Please assign AIBehavior in your AI controller BP")))
	{
		RunBehaviorTree(AIBehavior);
	}
}
