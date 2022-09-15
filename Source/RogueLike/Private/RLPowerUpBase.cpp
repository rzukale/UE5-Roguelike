// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPowerUpBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARLPowerUpBase::ARLPowerUpBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	bIsActive = true;
	RespawnTime = 10.0f;
	bReplicates = true;
	//SetReplicates(true);
}

void ARLPowerUpBase::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ARLPowerUpBase::SetPowerUpState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ARLPowerUpBase::HideAndCooldownPowerUp()
{
	SetPowerUpState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ARLPowerUpBase::ShowPowerUp, RespawnTime);
}

void ARLPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
}

FText ARLPowerUpBase::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ARLPowerUpBase::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ARLPowerUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLPowerUpBase, bIsActive);
}