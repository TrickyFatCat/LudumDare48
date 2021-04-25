// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interaction.h"

#include "FinalPortal.generated.h"

class USphereComponent;

UCLASS()
class LUDUMDARE48_API AFinalPortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFinalPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Trigger volume
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* TriggerVolume{nullptr};
};
