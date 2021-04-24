// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupHealPotion.h"

void APickupHealPotion::ActivatePickupEffect(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->IncreaseLives(HealPower);
}
