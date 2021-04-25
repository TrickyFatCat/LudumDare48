// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "Characters/PlayerCharacter.h"
#include "PickupCoin.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE48_API APickupCoin : public ABasePickup
{
	GENERATED_BODY()

public:
	virtual void ActivatePickup(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup|Coin", meta=(AllowPrivateAccess="true"))
	int32 Cost{1};
};
