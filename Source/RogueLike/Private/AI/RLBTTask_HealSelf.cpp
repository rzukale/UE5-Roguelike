// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLBTTask_HealSelf.h"

#include "AIController.h"
#include "RLAttributeComponent.h"

EBTNodeResult::Type URLBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}
	return EBTNodeResult::Succeeded;
}
