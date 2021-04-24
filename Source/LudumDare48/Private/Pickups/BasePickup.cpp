// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BasePickup.h"
#include "Characters/PlayerCharacter.h"


// Sets default values
ABasePickup::ABasePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = Mesh->GetComponentLocation();
}

// Called every frame
void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimatePosition();
	AnimateRotation();
}

void ABasePickup::AnimatePosition() const
{
	FVector CurrentLocation;
	const float Time = GetWorld()->GetTimeSeconds();
	CurrentLocation.X = InitialLocation.X + Amplitude.X * FMath::Sin(Frequency * Time);
	CurrentLocation.Y = InitialLocation.Y + Amplitude.Y * FMath::Sin(Frequency * Time);
	CurrentLocation.Z = InitialLocation.Z + Amplitude.Z * FMath::Sin(Frequency * Time);
	Mesh->SetWorldLocation(CurrentLocation);
}

void ABasePickup::AnimateRotation() const
{
	const FRotator NewRotation = Mesh->GetComponentRotation() + RotationSpeed;
	Mesh->SetWorldRotation(NewRotation);
}

void ABasePickup::ActivatePickup(APlayerCharacter* PlayerCharacter)
{
	ActivatePickupEffect(PlayerCharacter);
	OnPickup();
	Destroy();
}

void ABasePickup::ActivatePickupEffect(APlayerCharacter* PlayerCharacter)
{
}

