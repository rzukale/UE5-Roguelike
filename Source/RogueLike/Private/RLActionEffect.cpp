// Fill out your copyright notice in the Description page of Project Settings.


#include "RLActionEffect.h"
#include "RLActionComponent.h"
#include "GameFramework/GameStateBase.h"

URLActionEffect::URLActionEffect()
{
	bAutoStart = true;
}

void URLActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DurationHandle, Delegate, Duration, false);
	}
	if (TimeBetweenTicks > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeBetweenTicksHandle, Delegate, TimeBetweenTicks, true);
	}
}

void URLActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_TimeBetweenTicksHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);
	}


	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_TimeBetweenTicksHandle);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DurationHandle);

	URLActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}

float URLActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	return Duration;
}

void URLActionEffect::ExecutePeriodicEffect_Implementation(AActor* InstigatorActor)
{

}