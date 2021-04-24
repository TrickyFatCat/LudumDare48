// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Components/BaseResource.h"

#include "PlayerCharacter.generated.h"

class UBaseResource;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesDecreased, int32, Lives);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesIncreased, int32, Lives);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitPointsDecreased, int32, HitPoints);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitPointsIncreased, int32, HitPoints);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsDecreased, int32, Coins);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsIncreased, int32, Coins);
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

// Pickups
private:
	UFUNCTION()
	void ActivatePickup(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

// Lives
public:
	UFUNCTION(BlueprintCallable, Category="Player Character|Lives")
	void DecreaseLives(const int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category="Player Character|Lives")
	void IncreaseLives(const int32 Amount) const;
	UFUNCTION(BlueprintPure)
	int32 GetLives() const { return Lives->GetValue(); }
	UPROPERTY(BlueprintAssignable, Category="Player Character|Lives")
	FOnLivesDecreased OnLivesDecreased;
	UPROPERTY(BlueprintAssignable, Category="Player Character|Lives")
	FOnLivesIncreased OnLivesIncreased;
	UPROPERTY(BlueprintAssignable, Category="Player Character|Lives")
	FOnGameOver OnGameOver; // Called when lives <= 0
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Character|Lives", meta=(AllowPrivateAccess="true"))
	UBaseResource* Lives{nullptr};
	UFUNCTION()
	void BroadcastLivesDecreased(const int32 Value);
	UFUNCTION()
	void BroadcastLivesIncreased(const int32 Value);

// HitPoints
public:
	UFUNCTION(BlueprintCallable, Category="Player Character|HitPoints")
	void DecreaseHitPoints(const int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category="Player Character|HitPoints")
	void IncreaseHitPoints(const int32 Amount) const;
	UFUNCTION(BlueprintPure)
	int32 GetHitPoints() const { return HitPoints->GetValue(); }
	UFUNCTION(BlueprintPure)
	int32 GetMaxHitPoints() const { return HitPoints->GetMaxValue(); }
	UFUNCTION(BlueprintPure)
	bool GetIsInvulnerable() const { return HitPoints->GetIsImmune(); }
	UPROPERTY(BlueprintAssignable, Category="Player Character|HitPoints")
	FOnHitPointsDecreased OnHitPointsDecreased;
	UPROPERTY(BlueprintAssignable, Category="Player Character|HitPoints")
	FOnHitPointsIncreased OnHitPointsIncreased;
	UPROPERTY(BlueprintAssignable, Category="Player Character|HitPoints")
	FOnPlayerDeath OnPlayerDeath; // Called when hit points <= 0
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Character|HitPoints", meta=(AllowPrivateAccess="true"))
	UBaseResource* HitPoints{nullptr};
	UFUNCTION()
	void ReceiveDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);
	UFUNCTION()
	void BroadcastHitPointsDecreased(const int32 Value);
	UFUNCTION()
	void BroadcastHitPointsIncreased(const int32 Value);

// Coins
public:
	UFUNCTION(BlueprintCallable, Category="Player Character|Coins")
	void DecreaseCoins(const int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category="Player Character|Coins")
	void IncreaseCoins(const int32 Amount) const;
	UFUNCTION(BlueprintPure)
	int32 GetCoins() const { return Coins->GetValue(); }
	UPROPERTY(BlueprintAssignable, Category="Player Character|Coins")
	FOnCoinsDecreased OnCoinsDecreased;
	UPROPERTY(BlueprintAssignable, Category="Player Character|Coins")
	FOnCoinsIncreased OnCoinsIncreased;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Character|Coins", meta=(AllowPrivateAccess="true"))
	UBaseResource* Coins{nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Character|Coins", meta=(AllowPrivateAccess="true"))
	int32 LiveCost{100};
	UFUNCTION()
	void BroadcastCoinsDecreased(const int32 Value);
	UFUNCTION()
	void BroadcastCoinsIncreased(const int32 Value);
};
