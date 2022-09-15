// Fill out your copyright notice in the Description page of Project Settings.


#include "RLActionEffect_Thorns.h"

#include "RLActionComponent.h"
#include "RLAttributeComponent.h"
#include "RLGameplayFunctionLibrary.h"

URLActionEffect_Thorns::URLActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	TimeBetweenTicks = 0.0f;
}

void URLActionEffect_Thorns::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &URLActionEffect_Thorns::OnHealthChanged);
	}
}

void URLActionEffect_Thorns::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);

	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &URLActionEffect_Thorns::OnHealthChanged);
	}
}

void URLActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* Owner = GetOwningComponent()->GetOwner();

	if (Delta < 0.0f && Owner != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt32(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		ReflectedAmount = FMath::Abs(ReflectedAmount);

		URLGameplayFunctionLibrary::ApplyDamage(Owner, InstigatorActor, ReflectedAmount);
	}
}
