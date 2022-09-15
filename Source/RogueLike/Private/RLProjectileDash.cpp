// Fill out your copyright notice in the Description page of Project Settings.


#include "RLProjectileDash.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARLProjectileDash::ARLProjectileDash()
{
	TeleportDelay = 0.2f;
	ExplodeDelay = 0.2f;

	ProjectileMovementComp->InitialSpeed = 6000.0f;
}

void ARLProjectileDash::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedExplode, this, &ARLProjectileDash::Explode, ExplodeDelay);
}

void ARLProjectileDash::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedExplode);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	ParticleSystemComp->DeactivateSystem();
	ProjectileMovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ARLProjectileDash::TeleportInstigator, TeleportDelay);
}

void ARLProjectileDash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);

		APawn* InstigatorPawn = Cast<APawn>(ActorToTeleport);
		APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->ClientStartCameraShake(ImpactShake);
		}
	}
	Destroy();
}
