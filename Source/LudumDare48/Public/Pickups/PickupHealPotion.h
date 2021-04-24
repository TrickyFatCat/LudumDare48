// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupCoin.h"
#include "PickupHealPotion.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE48_API APickupHealPotion : public APickupCoin
{
	GENERATED_BODY()

protected:
	virtual  void ActivatePickupEffect(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup|HealPotion", meta=(AllowPrivateAccess="true"))
	int32 HealPower{1};
};
