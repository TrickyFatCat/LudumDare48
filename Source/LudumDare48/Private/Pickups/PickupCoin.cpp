// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupCoin.h"

void APickupCoin::ActivatePickupEffect(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->IncreaseCoins(Cost);
}
