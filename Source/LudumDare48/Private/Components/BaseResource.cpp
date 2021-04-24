// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseResource.h"


// Sets default values for this component's properties
UBaseResource::UBaseResource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseResource::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UBaseResource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseResource::IncreaseValue(const int32 Amount, const bool bClampToMax)
{
	Value += Amount;

	if (bClampToMax)
	{
		Value = FMath::Min(Value, MaxValue);
	}

	OnValueIncreased.Broadcast(Value);
}

void UBaseResource::DecreaseValue(const int32 Amount, const bool bActivateImmunity)
{
	if (bIsImmune) return;
	
	Value -= Amount;
	Value = FMath::Max(Value, 0);
	OnValueDecreased.Broadcast(Value);

	if (bActivateImmunity)
	{
		ActivateImmunity();
	}
}

void UBaseResource::ActivateImmunity()
{
	if (bIsImmune) return;

	bIsImmune = true;
	OnImmunityActivated.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(ImmunityTimerHandle, this, &UBaseResource::DeactivateImmunity, ImmunityDuration, false);
}

void UBaseResource::DeactivateImmunity()
{
	if (!bIsImmune) return;

	bIsImmune = false;
	OnImmunityDeactivated.Broadcast();
}
