// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupKey.h"

void APickupKey::ActivatePickup(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->AddKey(Color);
}
