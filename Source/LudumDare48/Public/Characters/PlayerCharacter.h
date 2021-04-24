// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE48_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Movement
private:
	UFUNCTION()
	void MoveForward(const float AxisValue);
	UFUNCTION()
	void MoveRight(const float AxisValue);
};
