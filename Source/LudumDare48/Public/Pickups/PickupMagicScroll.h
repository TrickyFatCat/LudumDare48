// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BasePickup.h"
#include "PickupMagicScroll.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE48_API APickupMagicScroll : public ABasePickup
{
	GENERATED_BODY()

public:
	virtual void ActivatePickup(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup|Magic Scroll", meta=(AllowPrivateAccess="true"))
	int32 MagicRestore{1};
};
