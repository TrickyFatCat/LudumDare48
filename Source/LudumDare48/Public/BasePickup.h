// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UTimelineComponent;
class APlayerCharacter;

UCLASS()
class LUDUMDARE48_API ABasePickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Trigger volume
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Pickup|TriggerVolume", meta=(AllowPrivateAccess="true"))
	USphereComponent* TriggerVolume{nullptr};
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Pickup|Mesh", meta=(AllowPrivateAccess="true"));
	UStaticMeshComponent* Mesh{nullptr};
	
// Animation
private:
	FVector InitialLocation{FVector::ZeroVector};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Animation", meta=(AllowPrivateAccess="true"))
	FVector Amplitude{FVector(0.f, 0.f, 50.f)};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Animation", meta=(AllowPrivateAccess="true"))
	float Frequency{25.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Animation", meta=(AllowPrivateAccess="true"))
	FRotator RotationSpeed{FRotator(0.f, 35.f, 0.f)};
	void AnimatePosition() const;
	void AnimateRotation() const;

// Pickup effect
public:
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void ActivatePickup(APlayerCharacter* PlayerCharacter);
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickup();
	virtual void ActivatePickupEffect(APlayerCharacter* PlayerCharacter);
};
