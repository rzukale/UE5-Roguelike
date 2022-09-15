// Fill out your copyright notice in the Description page of Project Settings.


#include "RLTargetDummy.h"

#include "RLAttributeComponent.h"

// Sets default values
ARLTargetDummy::ARLTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");

}

void ARLTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARLTargetDummy::OnHealthChanged);
}

void ARLTargetDummy::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}


