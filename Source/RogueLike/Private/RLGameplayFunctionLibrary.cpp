// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayFunctionLibrary.h"

#include "RLAttributeComponent.h"

bool URLGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
    URLAttributeComponent* AttributeComponent = URLAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComponent)
    {
        return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
    }
    return false;
}

bool URLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
    {
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            // Direction = Target - Origin
            FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
            Direction.Normalize();
            HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
        }
        return true;
    }
    return false;
}
