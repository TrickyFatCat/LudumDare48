// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BaseRoom.generated.h"

UCLASS()
class LUDUMDARE48_API ABaseRoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Root", meta=(AllowPrivateAccess="true"))
	USceneComponent* Floor{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Floor", meta=(AllowPrivateAccess="true"))
	USceneComponent* CameraAnchor{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerSpawn", meta=(AllowPrivateAccess="true"))
	TArray<UArrowComponent*> PlayerSpawnPoints{};
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = "true"))
	// UCameraComponent* Camera{nullptr};
};
