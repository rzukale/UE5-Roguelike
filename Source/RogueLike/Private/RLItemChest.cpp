// Fill out your copyright notice in the Description page of Project Settings.


#include "RLItemChest.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ARLItemChest::ARLItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
	GoldMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
	bReplicates = true;
	//SetReplicates(true);
}

void ARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ARLItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ARLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLItemChest, bLidOpened);
}

void ARLItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.0f, 0.0f));
}