// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMagicProjectile.h"

#include "RLActionComponent.h"
#include "RLGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RLActionEffect.h"

ARLMagicProjectile::ARLMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	DamageAmount = 20.0f;
}

void ARLMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARLMagicProjectile::OnActorOverlap);
}

void ARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		URLActionComponent* ActionComp = Cast<URLActionComponent>(OtherActor->GetComponentByClass(URLActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			ProjectileMovementComp->Velocity = -ProjectileMovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		if (URLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			if (ActionComp && BurningActionClass && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
			Explode();
		}
	}
}
