// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonster::MoveMonster()
{
	FNode* Current = Grid[Position.PositionX][Position.PositionY];

	int r = FMath::RandRange(0,Current->Edges.size() - 1);
	FNode* Next = Current->Edges[r];
	Position = Next->Value->Position();
	FVector Location = Next->Value->GetActorLocation();
	Location.Z = 300.0f;

	SetActorLocation(Location);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

