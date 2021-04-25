// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/FinalPortal.h"
#include "Components/SphereComponent.h"


// Sets default values
AFinalPortal::AFinalPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetSphereRadius(128.f);
	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void AFinalPortal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFinalPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}