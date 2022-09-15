// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

URLAction_ProjectileAttack::URLAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimationDelay = 0.3f;

	SweepRadius = 20.0f;
	SweepDistanceFallback = 5000.0f;
}

void URLAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnimation);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		UGameplayStatics::SpawnSoundAttached(CastingSound, Character->GetMesh());

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimationDelay, false);
		}
	}
}

void URLAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
		FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();
		FTransform SpawnTransform = FTransform(ProjRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}
