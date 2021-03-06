// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Interaction.h"
#include "Pickups/BasePickup.h"


DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All)

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AIControllerClass = nullptr;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	Lives = CreateDefaultSubobject<UBaseResource>(TEXT("Lives"));
	Lives->SetMaxValue(3);
	
	HitPoints = CreateDefaultSubobject<UBaseResource>(TEXT("HitPoints"));
	HitPoints->SetMaxValue(3);
	
	Coins = CreateDefaultSubobject<UBaseResource>(TEXT("Coins"));
	Coins->SetMaxValue(0);

	Magic = CreateDefaultSubobject<UBaseResource>(TEXT("Magic"));
	Magic->SetMaxValue(3);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::ActivatePickup);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::AddToInteractionQueue);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::RemoveFromInteractionQueue);

	Lives->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastLivesDecreased);
	Lives->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastLivesIncreased);

	HitPoints->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastHitPointsDecreased);
	HitPoints->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastHitPointsIncreased);
	// OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::ReceiveDamage);

	Coins->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastCoinsDecreased);
	Coins->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastCoinsIncreased);

	Magic->OnValueDecreased.AddDynamic(this, &APlayerCharacter::BroadcastMagicDecreased);
	Magic->OnValueIncreased.AddDynamic(this, &APlayerCharacter::BroadcastMagicIncreased);

	InitialLocation = GetActorLocation();
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
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Cast", IE_Pressed, this, &APlayerCharacter::CastMagic);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
}


void APlayerCharacter::UpdatePositions(const FRoomPosition PlayerNewPosition, const FRoomPosition MonsterNewPosition)
{
	Position = PlayerNewPosition;
	MonsterPosition = MonsterNewPosition;
	OnRoomChanged.Broadcast();

	UE_LOG(LogPlayerCharacter, Warning, TEXT("Position Player : x%i y%i"), Position.PositionX, Position.PositionY);
	UE_LOG(LogPlayerCharacter, Warning, TEXT("Position Monster : x%i y%i"), MonsterPosition.PositionX, MonsterPosition.PositionY);

	if(
		(std::abs(Position.PositionX - MonsterPosition.PositionX) == 1 || std::abs(Position.PositionY - MonsterPosition.PositionY) == 1) &&
		!(std::abs(Position.PositionX - MonsterPosition.PositionX) == 1 && std::abs(Position.PositionY - MonsterPosition.PositionY) == 1)
	)
	{
		OnMonsterNearby.Broadcast(true);
		return;
	}
	
	OnMonsterNearby.Broadcast(false);
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

void APlayerCharacter::ActivatePickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABasePickup* Pickup = Cast<ABasePickup>(OtherActor);

	if (Pickup == nullptr) return;

	Pickup->ActivatePickup(this);
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

	if (GetLives() <= 0) OnGameOver.Broadcast();
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
	DecreaseLives(Damage);
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

void APlayerCharacter::DecreaseCoins(const int32 Amount) const
{
	Coins->DecreaseValue(Amount, false);
}

void APlayerCharacter::IncreaseCoins(const int32 Amount) const
{
	Coins->IncreaseValue(Amount, false);
}

void APlayerCharacter::BroadcastCoinsDecreased(const int32 Value)
{
	OnCoinsIncreased.Broadcast(Value);
}

void APlayerCharacter::BroadcastCoinsIncreased(const int32 Value)
{
	OnCoinsDecreased.Broadcast(Value);
	
	if (Value >= LiveCost)
	{
		DecreaseCoins(LiveCost);
		IncreaseLives(1);
	}
}

void APlayerCharacter::DecreaseMagic(const int32 Amount) const
{
	Magic->DecreaseValue(Amount, false);
}

void APlayerCharacter::IncreaseMagic(const int32 Amount) const
{
	Magic->IncreaseValue(Amount, false);
}

void APlayerCharacter::BroadcastMagicDecreased(const int32 Value)
{
	OnMagicDecreased.Broadcast(Value);
}

void APlayerCharacter::BroadcastMagicIncreased(const int32 Value)
{
	OnMagicIncreased.Broadcast(Value);
}

void APlayerCharacter::CastMagic()
{
	int32 PreviousMagic = GetMagic();
	DecreaseMagic(1);

	if (PreviousMagic > 0)
	{
		OnMagicCasted.Broadcast();
	}
	// Do what you must to do
	// Here we must play our anim montage
}

void APlayerCharacter::AddKey(const EKeyColor KeyColor)
{
	if (Keys[KeyColor]) return;

	Keys.Emplace(KeyColor, true);
	OnKeyAdded.Broadcast(KeyColor);
}

bool APlayerCharacter::HasKey(const EKeyColor KeyColor)
{
	if (Keys.Num() == 0 || Keys.Find(KeyColor) == nullptr) return false;
	
	return static_cast<bool>(Keys.Find(KeyColor));
}

void APlayerCharacter::AddToInteractionQueue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) return;

	if (!OtherActor->GetClass()->ImplementsInterface(UInteraction::StaticClass())) return;

	InteractionQueue.Add(OtherActor);
}

void APlayerCharacter::RemoveFromInteractionQueue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractionQueue.Find(OtherActor))
	{
		InteractionQueue.Remove(OtherActor);
	}
}

void APlayerCharacter::Interact()
{
	if (InteractionQueue.Num() == 0) return;

	AActor* TargetActor = InteractionQueue.Array()[0];

	if (!IsValid(TargetActor)) return;

	while (!IsValid(TargetActor))
	{
		InteractionQueue.Remove(TargetActor);
		TargetActor = InteractionQueue.Array()[0];

		if (InteractionQueue.Num() == 0) return;
	}
	
	const bool InteractionResult = IInteraction::Execute_ProcessInteraction(TargetActor, this);
}
