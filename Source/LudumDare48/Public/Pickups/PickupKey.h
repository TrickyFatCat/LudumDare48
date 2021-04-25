// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "Characters/PlayerCharacter.h"
#include "PickupKey.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE48_API APickupKey : public ABasePickup
{
	GENERATED_BODY()

public:
	virtual void ActivatePickup(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Pickup|Key")
	EKeyColor Color{EKeyColor::RED};
};
