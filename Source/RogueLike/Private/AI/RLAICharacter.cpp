// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RLAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "RLAttributeComponent.h"
#include "BrainComponent.h"
#include "RLActionComponent.h"
#include "RLWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARLAICharacter::ARLAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ActionComp = CreateDefaultSubobject<URLActionComponent>("ActionComp");
	TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ARLAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ARLAICharacter::OnHealthChanged);
}

void ARLAICharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<URLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		if (Delta < 20.f && NewHealth > 0) // don't play sounds at minor DOT damage
		{
			// play sound que of damaged entity
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		if (NewHealth <= 0.0f) // dead
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// play death sound que
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.0f);
		}
	}
}

void ARLAICharacter::OnPawnSeen(APawn* OtherPawn)
{
	if (GetTargetActor() != OtherPawn)
	{
		SetTargetActor(OtherPawn);
		MulticastPawnSeen();
	}
}

void ARLAICharacter::MulticastPawnSeen_Implementation()
{
	URLWorldUserWidget* NewWidget = CreateWidget<URLWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		NewWidget->AddToViewport(10);
	}
}

AActor* ARLAICharacter::GetTargetActor() const
{
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		return Cast<AActor>(MyController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void ARLAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		MyController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}


