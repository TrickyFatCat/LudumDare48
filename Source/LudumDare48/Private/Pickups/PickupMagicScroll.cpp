// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupMagicScroll.h"

#include "Characters/PlayerCharacter.h"

void APickupMagicScroll::ActivatePickup(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->IncreaseMagic(MagicRestore);
	Super::ActivatePickup(PlayerCharacter);
}
