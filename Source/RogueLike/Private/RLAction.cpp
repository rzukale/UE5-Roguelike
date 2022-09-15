// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAction.h"

#include "RLActionComponent.h"
#include "../RogueLike.h"
#include "Net/UnrealNetwork.h"

void URLAction::Initialize(URLActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
	RepData.bIsRunning = false;
	RepData.Instigator = nullptr;
}

void URLAction::StartAction_Implementation(AActor* InstigatorActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Running %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	URLActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = InstigatorActor;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void URLAction::StopAction_Implementation(AActor* InstigatorActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);

	URLActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = InstigatorActor;

	GetOwningComponent()->OnActionEnded.Broadcast(GetOwningComponent(), this);
}

bool URLAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (IsRunning())
	{
		return false;
	}

	URLActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

UWorld* URLAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

URLActionComponent* URLAction::GetOwningComponent() const
{
	return ActionComponent;
}

void URLAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool URLAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool URLAction::IsSupportedForNetworking() const
{
	return true;
}

void URLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLAction, RepData);
	DOREPLIFETIME(URLAction, ActionComponent);
	DOREPLIFETIME(URLAction, TimeStarted);
}