// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	if (!IsValid(Controller)) return;

	if (AxisValue != 0.f)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator NewYawRotation(0.f, ControllerRotation.Yaw, 0.f);
		const FVector MoveDirection = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(MoveDirection, AxisValue);
	}
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	if (!IsValid(Controller)) return;

	if (AxisValue != 0.f)
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator NewYawRotation(0.f, ControllerRotation.Yaw, 0.f);
		const FVector MoveDirection = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(MoveDirection, AxisValue);
	}
}


