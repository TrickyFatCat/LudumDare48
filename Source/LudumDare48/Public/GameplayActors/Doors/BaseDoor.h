// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/PlayerCharacter.h"
#include "BaseDoor.generated.h"

class UTimelineComponent;


UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Opened,
	Closed,
	Locked,
	Transition,
	Disabled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpened);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorClosed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorLocked);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorUnlocked);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorDisabled);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorEnabled);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorTransitionStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorTransitionReverted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorTransitionFinished);

UCLASS()
class LUDUMDARE48_API ABaseDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

// Door controls
public:
	UFUNCTION(BlueprintCallable, Category="Door")
	bool OpenDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool CloseDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool LockDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool UnlockDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool DisableDoor();
	UFUNCTION(BlueprintCallable, Category="Door")
	bool EnableDoor(const EDoorState NewState);

// Door objects
protected:
	UFUNCTION(BlueprintCallable, Category="Door|Objects")
	void FillLeavesData();
	UFUNCTION(BlueprintCallable, Category="Door|Objects")
	void FillInitialTransforms();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* DoorFrame{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* DoorRoot{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door|Objects", meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> DoorMeshes{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door|Objects", meta=(AllowPrivateAccess="true"))
	TMap<USceneComponent*, FTransform> InitialTransforms{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door|Objects", meta=(AllowPrivateAccess="true"))
	TArray<FTransform> TargetTransforms{};
	UFUNCTION()
	void SetDoorTransform(const float Progress);
// Door States
public:
	UFUNCTION(BlueprintPure, Category="Door|States")
	EDoorState GetInitialState() const { return InitialState; }
	UFUNCTION(BlueprintCallable, Category="Door|States")
	void SetInitialState(const EDoorState NewState) { InitialState = NewState; }
	UFUNCTION(BlueprintPure, Category="Door|Statets")
	EDoorState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintPure, Category="Door|Statets")
	EDoorState GetTargetState() const { return TargetState; }
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorOpened OnDoorOpened;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorClosed OnDoorClosed;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorLocked OnDoorLocked;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorUnlocked OnDoorUnlocked;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorDisabled OnDoorDisabled;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorEnabled OnDoorEnabled;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorTransitionStarted OnTransitionStarted;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorTransitionReverted OnDoorTransitionReverted;
	UPROPERTY(BlueprintAssignable, Category="Door|States")
	FOnDoorTransitionFinished OnDoorTransitionFinished;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Door|States")
	void OnStateChanged();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category="Door|States", meta=(AllowPrivateAccess="true"))
	EDoorState InitialState{EDoorState::Closed};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter=GetCurrentState, Category="Door|States", meta=(AllowPrivateAccess="true"))
	EDoorState CurrentState{InitialState};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door|States", meta=(AllowPrivateAccess="true"))
	EDoorState PreviousState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door|States", meta=(AllowPrivateAccess="true"))
	EDoorState TargetState;
	UFUNCTION()
	void ChangeState(const EDoorState NewState);
	void SetTargetState(const EDoorState NewState);

// Door animation
public:
	UFUNCTION(BlueprintPure, Category="Door|Animation")
	float GetAnimationDuration() const { return AnimationDuration; }
	UFUNCTION(BlueprintPure, Category="Door|Animation")
	bool GetIsRevertible() const { return bIsRevertible; }
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Door|Components", meta=(AllowPrivateAccess="true"))
	UTimelineComponent* AnimationTimeline{nullptr};
	UPROPERTY(EditAnywhere, Category="Door|Animation", meta=(AllowPrivateAcccess="true"))
	UCurveFloat* AnimationCurve{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter=GetAnimationDuration, Category="Door|Animation", meta=(AllowPrivateAccess="true"))
	float AnimationDuration{0.25f};
	UFUNCTION()
	void CalculatePlayRate() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter=GetIsRevertible, Category="Door|Animation", meta=(AllowPrivateAccess="true"))
	bool bIsRevertible{false};
	UFUNCTION()
	void StartTransition();
	UFUNCTION()
	void RevertTransition();
	UFUNCTION()
	void FinishTransition();

// Close Delay
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Behaviour", meta=(AllowPrivateAccess="true"))
	TSet<EKeyColor> RequiredKeys{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Behaviour", meta=(AllowPrivateAccess="true"))
	float CloseDelay{0.f};
	UPROPERTY(BlueprintReadOnly, Category="Door|Behaviour", meta=(AllowPrivateAccess="true"))
	FTimerHandle CloseDelayHandle;
	
};

