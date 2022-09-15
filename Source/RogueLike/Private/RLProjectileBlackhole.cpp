// Fill out your copyright notice in the Description page of Project Settings.


#include "RLProjectileBlackhole.h"

#include "RLActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "RLActionEffect.h"
#include "RLGameplayFunctionLibrary.h"
#include "../RogueLike.h"

ARLProjectileBlackhole::ARLProjectileBlackhole()
{
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(SphereComp);
	OuterSphereComp = CreateDefaultSubobject<USphereComponent>("OuterSphereComp");
	OuterSphereComp->SetupAttachment(SphereComp);
	OuterSphereComp->SetCollisionProfileName("Projectile");
	//OuterSphereComp->SetGenerateOverlapEvents(true);
	OuterSphereComp->SetSphereRadius(1000.0f);
	ForceComp->ForceStrength = -2000000.0f;
	ForceComp->Radius = 1000.0f;
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	SphereComp->SetSphereRadius(55.0f);
	InitialLifeSpan = 5.0f;
}

void ARLProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OuterSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARLProjectileBlackhole::OnPawnOverlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARLProjectileBlackhole::OnActorOverlap);
}

void ARLProjectileBlackhole::BeginPlay()
{
	Super::BeginPlay();
}

void ARLProjectileBlackhole::OnPawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//LogOnScreen(this, FString::Printf(TEXT("Overlapped with '%s'"), *GetNameSafe(OtherActor)));
		URLActionComponent* ActionComp = Cast<URLActionComponent>(OtherActor->GetComponentByClass(URLActionComponent::StaticClass()));
		if (URLGameplayFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, 10))
		{
			if (ActionComp && !ActionComp->ActiveGameplayTags.HasTag(CrushingTag) && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), CrushingActionClass);
			}
		}

	}
}

void ARLProjectileBlackhole::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		LogOnScreen(this, FString::Printf(TEXT("Overlapped with '%s'"), *GetNameSafe(OtherActor)));
		if (OtherActor)
		{
			OtherActor->Destroy();
		}
	}
}
