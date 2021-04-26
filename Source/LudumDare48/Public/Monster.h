// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Graph.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Room/Room.h"

#include "Monster.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogMonster, All, All)

UCLASS()
class LUDUMDARE48_API AMonster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FRoomPosition Position;
	
	FGraph* Graph;
	TArray<TArray<FNode*>> Grid;

	UFUNCTION(BlueprintCallable)
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void OnPlayerTeleported(ARoom* NewRoom, APlayerCharacter* Player);
};
