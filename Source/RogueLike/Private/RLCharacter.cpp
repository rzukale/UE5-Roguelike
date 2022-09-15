// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacter.h"

#include "RLActionComponent.h"
#include "RLAttributeComponent.h"
#include "RLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARLCharacter::ARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<URLInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<URLActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	TimeToHitParamName = "TimeToHit";
}

void ARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARLCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARLCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ARLCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ARLCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ARLCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ARLCharacter::Blackhole()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void ARLCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ARLCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ARLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ARLCharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth > 0)
		{
			// play damaged sound que
		}

		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		DisableInput(GetController<APlayerController>());
		// death sound here
		SetLifeSpan(5.0f);
	}
}

// Called to bind functionality to input
void ARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARLCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARLCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ARLCharacter::Dash);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &ARLCharacter::Blackhole);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARLCharacter::SprintStop);
}

void ARLCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FVector ARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}
