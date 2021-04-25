// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Doors/BaseDoor.h"

#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseDoor::ABaseDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AnimationTimeline"));

	DoorRoot= CreateDefaultSubobject<USceneComponent>(TEXT("LeavesAnchor"));
	RootComponent = DoorRoot;
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();

	if (AnimationCurve != nullptr)
	{
		FOnTimelineFloat AnimationProgress;
		AnimationProgress.BindUFunction(this, FName("SetDoorTransform"));
		AnimationTimeline->AddInterpFloat(AnimationCurve, AnimationProgress);
		FOnTimelineEvent AnimationFinish;
		AnimationFinish.BindUFunction(this, FName("FinishTransition"));
		AnimationTimeline->SetTimelineFinishedFunc(AnimationFinish);
		CalculatePlayRate();
	}

	FillLeavesData();
	FillInitialTransforms();
}

// Called every frame
void ABaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseDoor::OpenDoor()
{
	if (RequiredKeys.Num() > 0)
	{
		bool bCanOpen = true;
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
		
		for(EKeyColor Key : RequiredKeys)
		{
			if (!Player->GetKeys()[Key])
			{
				bCanOpen = false;
				break;
			}
		}

		if (!bCanOpen) return false;
	}
	
	if (CurrentState == EDoorState::Locked || CurrentState == EDoorState::Disabled || CurrentState == EDoorState::Opened
)
	{
		return false;
	}

	if (bIsRevertible && CurrentState == EDoorState::Transition && TargetState == EDoorState::Closed)
	{
		RevertTransition();
		return true;
	}

	if (!bIsRevertible && CurrentState == EDoorState::Transition) return false;

	if (AnimationDuration <= 0.f)
	{
		SetTargetState(CurrentState);
		FinishTransition();
		return true;
	}

	StartTransition();
	return true;
}

bool ABaseDoor::CloseDoor()
{
	if (CurrentState == EDoorState::Locked || CurrentState == EDoorState::Disabled || CurrentState == EDoorState::Closed
	)
	{
		return false;
	}

	if (bIsRevertible && CurrentState == EDoorState::Transition && TargetState == EDoorState::Opened)
	{
		RevertTransition();
		return true;
	}

	if (!bIsRevertible && CurrentState == EDoorState::Transition) return false;

	if (GetWorld()->GetTimerManager().IsTimerActive(CloseDelayHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseDelayHandle);
	}

	if (AnimationDuration <= 0.f)
	{
		SetTargetState(CurrentState);
		FinishTransition();
		return true;
	}

	StartTransition();
	return true;
}

bool ABaseDoor::LockDoor()
{
	if (CurrentState == EDoorState::Locked || CurrentState != EDoorState::Closed)
	{
		return false;
	}

	ChangeState(EDoorState::Locked);
	return true;
}

bool ABaseDoor::UnlockDoor()
{
	if (CurrentState != EDoorState::Locked)
	{
		return false;
	}

	CurrentState = EDoorState::Closed;
	OnDoorUnlocked.Broadcast();
	return true;
}

bool ABaseDoor::DisableDoor()
{
	if (CurrentState == EDoorState::Closed || CurrentState == EDoorState::Opened)
	{
		ChangeState(EDoorState::Disabled);
		return true;
	}

	return false;
}

bool ABaseDoor::EnableDoor(const EDoorState NewState)
{
	if (CurrentState != EDoorState::Disabled)
	{
		return false;
	}

	CurrentState = NewState;
	OnDoorEnabled.Broadcast();
	return true;
}

void ABaseDoor::FillLeavesData()
{
	DoorMeshes.Empty();
	DoorFrame->GetChildrenComponents(false, DoorMeshes);
}

void ABaseDoor::FillInitialTransforms()
{
	InitialTransforms.Empty();

	for(USceneComponent* DoorLeaf : DoorMeshes)
	{
		InitialTransforms.Add(DoorLeaf, DoorLeaf->GetComponentTransform());
	}
}

void ABaseDoor::SetDoorTransform(const float Progress)
{
	if (InitialTransforms.Num() == 0 || TargetTransforms.Num() == 0 || DoorMeshes.Num() == 0) return;

	int32 TransformIndex = 0;

	for (USceneComponent* DoorLeaf : DoorMeshes)
	{
		FTransform NewTransform = InitialTransforms[DoorLeaf];
		FTransform TargetTransform = TargetTransforms[TransformIndex++];
		NewTransform.SetLocation(NewTransform.GetLocation() + TargetTransform.GetLocation() * Progress);
		NewTransform.SetRotation(NewTransform.GetRotation() + TargetTransform.GetRotation() * Progress);
		NewTransform.SetScale3D(NewTransform.GetScale3D() + TargetTransform.GetScale3D() * Progress);
		DoorLeaf->SetWorldTransform(NewTransform);
	}
}

void ABaseDoor::ChangeState(const EDoorState NewState)
{
	PreviousState = CurrentState;
	CurrentState = NewState;

	switch (CurrentState)
	{
		case EDoorState::Opened:
			OnDoorOpened.Broadcast();

			if (CloseDelay > 0.f)
			{
				GetWorldTimerManager().SetTimer(CloseDelayHandle, this, &ABaseDoor::StartTransition, CloseDelay, false);
			}
			break;
		case EDoorState::Closed:
			OnDoorClosed.Broadcast();
			break;
		case EDoorState::Locked:
			OnDoorLocked.Broadcast();
			break;
		case EDoorState::Disabled:
			OnDoorDisabled.Broadcast();
			break;
		case EDoorState::Transition:
			OnTransitionStarted.Broadcast();
			break;
	}

	OnStateChanged();
}

void ABaseDoor::SetTargetState(const EDoorState NewState)
{
	if (NewState == EDoorState::Closed)
	{
		TargetState = EDoorState::Opened;
	}
	else
	{
		TargetState = EDoorState::Closed;
	}
}

void ABaseDoor::CalculatePlayRate() const
{
	if (AnimationCurve == nullptr) return;

	AnimationTimeline->SetPlayRate(1 / AnimationDuration);
}

void ABaseDoor::StartTransition()
{
	SetTargetState(CurrentState);
	FTimerManager& TimerManager = GetWorldTimerManager();

	if (TimerManager.IsTimerActive(CloseDelayHandle))
	{
		TimerManager.ClearTimer(CloseDelayHandle);
	}

	if (TargetState == EDoorState::Opened) AnimationTimeline->PlayFromStart();
	if (TargetState == EDoorState::Closed) AnimationTimeline->ReverseFromEnd();
	
	ChangeState(EDoorState::Transition);
}

void ABaseDoor::RevertTransition()
{
	SetTargetState(TargetState);
	if (TargetState == EDoorState::Opened) AnimationTimeline->Play();
	if (TargetState == EDoorState::Closed) AnimationTimeline->Reverse();
	OnDoorTransitionReverted.Broadcast();
}

void ABaseDoor::FinishTransition()
{
	ChangeState(TargetState);
	OnDoorTransitionFinished.Broadcast();
}
