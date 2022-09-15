// Fill out your copyright notice in the Description page of Project Settings.


#include "RLExplosiveBarrel.h"

#include "RLAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARLExplosiveBarrel::ARLExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->SetAutoActivate(false);
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	ForceComp->bImpulseVelChange = true;
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ARLExplosiveBarrel::OnActorHit);
}

void ARLExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	if (OtherActor)
	{
		URLAttributeComponent* AttributeComp = Cast<URLAttributeComponent>(OtherActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(this, -50.0f);
		}
	}
}
