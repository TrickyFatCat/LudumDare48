// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseResource.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueIncreased, int32, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueDecreased, int32, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImmunityActivated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImmunityDeactivated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUMDARE48_API UBaseResource : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseResource();

	UPROPERTY(BlueprintAssignable, Category="Base Resource")
	FOnValueIncreased OnValueIncreased;
	UPROPERTY(BlueprintAssignable, Category="Base Resource")
	FOnValueDecreased OnValueDecreased;
	UPROPERTY(BlueprintAssignable, Category="Base Resource")
	FOnImmunityActivated OnImmunityActivated;
	UPROPERTY(BlueprintAssignable, Category="Base Resource")
	FOnImmunityDeactivated OnImmunityDeactivated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void IncreaseValue(const int32 Amount, const bool bClampToMax);
	UFUNCTION(BlueprintCallable)
	void DecreaseValue(const int32 Amount, const bool bActivateImmunity);
	UFUNCTION(BlueprintPure)
	int32 GetValue() const { return Value; }
	UFUNCTION(BlueprintPure)
	int32 GetMaxValue() const { return MaxValue; }
	UFUNCTION(BlueprintCallable)
	void SetMaxValue(const int32 NewValue) { MaxValue = NewValue; }
	UFUNCTION(BlueprintPure)
	bool GetIsImmune() const { return bIsImmune; }
	UFUNCTION(BlueprintPure)
	float GetImmunityDuration() const { return ImmunityDuration; }
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter=GetMaxValue, BlueprintSetter=SetMaxValue, Category="Base Resource", meta=(AllowPrivateAccess="true", ClampMin="0"))
	int32 MaxValue{3};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter=GetValue, Category="Base Resource", meta=(AllowPrivateAccess="true"))
	int32 Value{3};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter=GetImmunityDuration, Category="Base Resource", meta=(AllowPrivateAccess="true"))
	float ImmunityDuration{3.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter=GetIsImmune, Category="Base Resource", meta=(AllowPrivateAccess="true"))
	bool bIsImmune{false};
	UPROPERTY()
	FTimerHandle ImmunityTimerHandle;
	UFUNCTION()
	void ActivateImmunity();
	UFUNCTION()
	void DeactivateImmunity();
};
