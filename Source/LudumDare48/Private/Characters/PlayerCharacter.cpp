// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AIControllerClass = nullptr;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Lives->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastLivesDecreased);
	Lives->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastLivesIncreased);

	HitPoints->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastHitPointsDecreased);
	HitPoints->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastHitPointsIncreased);
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::ReceiveDamage);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Lives = CreateDefaultSubobject<UBaseResource>(TEXT("Lives"));
	HitPoints = CreateDefaultSubobject<UBaseResource>(TEXT("HitPoints"));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
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

void APlayerCharacter::DecreaseLives(const int32 Amount) const
{
	Lives->DecreaseValue(Amount, false);
}

void APlayerCharacter::IncreaseLives(const int32 Amount) const
{
	Lives->IncreaseValue(Amount, false);
}

void APlayerCharacter::BroadcastLivesDecreased(const int32 Value)
{
	OnLivesDecreased.Broadcast(Value);
}

void APlayerCharacter::BroadcastLivesIncreased(const int32 Value)
{
	OnLivesIncreased.Broadcast(Value);
}

void APlayerCharacter::DecreaseHitPoints(const int32 Amount) const
{
	HitPoints->DecreaseValue(Amount, true);

	if (GetHitPoints() <= 0)
	{
		DecreaseLives(1);
	}
}

void APlayerCharacter::IncreaseHitPoints(const int32 Amount) const
{
	HitPoints->IncreaseValue(Amount, true);
}

void APlayerCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	DecreaseHitPoints(Damage);
}

void APlayerCharacter::BroadcastHitPointsDecreased(const int32 Value)
{
	OnHitPointsDecreased.Broadcast(Value);

	if (Value <= 0)
	{
		OnPlayerDeath.Broadcast();
	}
}

void APlayerCharacter::BroadcastHitPointsIncreased(const int32 Value)
{
	OnHitPointsIncreased.Broadcast(Value);
}
