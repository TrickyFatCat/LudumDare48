// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonster::OnPlayerTeleported(ARoom* NewRoom, APlayerCharacter* Player)
{

	FNode* Current = Grid[Position.PositionX][Position.PositionY];

	int r = FMath::RandRange(0,Current->Edges.size() - 1);
	FNode* Next = Current->Edges[r];
	Position = Next->Value->Position();
	UE_LOG(LogMonster, Warning, TEXT("Monstro Position: x=%i y=%i"), Next->Value->Position().PositionX, Next->Value->Position().PositionY);
	UE_LOG(LogMonster, Warning, TEXT("Player Position: x=%i y=%i"), NewRoom->Position().PositionX, NewRoom->Position().PositionY);

	if(NewRoom->Position().PositionX == Next->Value->Position().PositionX && NewRoom->Position().PositionY == Next->Value->Position().PositionY)
	{
		PlayerCharacter->DecreaseLives(1);
	}
	
	FVector Location = Next->Value->GetActorLocation();
	Location.Z = 150.0f;

	SetActorLocation(Location);
}

