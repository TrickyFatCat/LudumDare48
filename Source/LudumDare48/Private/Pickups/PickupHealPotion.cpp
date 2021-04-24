// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupHealPotion.h"
#include "Characters/PlayerCharacter.h"

void APickupHealPotion::ActivatePickup(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter->GetHitPoints() == PlayerCharacter->GetMaxHitPoints()) return;
	
	PlayerCharacter->IncreaseLives(HealPower);
	Super::ActivatePickup(PlayerCharacter);
}
