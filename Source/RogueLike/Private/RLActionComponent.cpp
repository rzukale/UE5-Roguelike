// Fill out your copyright notice in the Description page of Project Settings.


#include "RLActionComponent.h"
#include "RLAction.h"
#include "../RogueLike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_RHZ);

// Sets default values for this component's properties
URLActionComponent::URLActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void URLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<URLAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void URLActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<URLAction*> ActionsCopy = Actions;
	for (URLAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void URLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
	/*for (URLAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMessage = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, ActionMessage, TextColor, 0.0f);
	}*/
}

void URLActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<URLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	URLAction* NewAction = NewObject<URLAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(InstigatorActor)))
		{
			NewAction->StartAction(InstigatorActor);
		}
	}
}

bool URLActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (URLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(InstigatorActor))
			{
				FString DebugMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(InstigatorActor, ActionName);
			}

			// bookmark for Unreal insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

bool URLActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (URLAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(InstigatorActor, ActionName);
				}
				Action->StopAction(InstigatorActor);
				return true;
			}
		}
	}
	return false;
}

void URLActionComponent::RemoveAction(URLAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

URLAction* URLActionComponent::GetAction(TSubclassOf<URLAction> ActionClass) const
{
	for (URLAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}

void URLActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}

void URLActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StopActionByName(InstigatorActor, ActionName);
}

void URLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLActionComponent, Actions);
}

bool URLActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bAllowsReplication = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URLAction* Action : Actions)
	{
		if (Action)
		{
			bAllowsReplication |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return bAllowsReplication;
}