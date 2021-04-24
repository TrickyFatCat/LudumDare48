// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupCoin.h"

void APickupCoin::ActivatePickup(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->IncreaseCoins(Cost);
	Super::ActivatePickup(PlayerCharacter);
}
