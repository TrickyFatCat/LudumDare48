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
	FNode* CurrentMonsterNode = Grid[Position.PositionX][Position.PositionY];
	FNode* PlayerNode = Grid[NewRoom->Position().PositionX][NewRoom->Position().PositionY];
	const std::deque<FNode*> PathToPlayer = Graph->PathBfs(CurrentMonsterNode, PlayerNode);

	FNode* Next = nullptr;
	if (PathToPlayer.size() != 0) Next = PathToPlayer[PathToPlayer.size() - 2];

	if(Next && FMath::RandRange(0, 10) <= 4)
	{
		Next = CurrentMonsterNode->Edges[FMath::RandRange(0, CurrentMonsterNode->Edges.size() - 1)];	
	}

	Position = Next->Value->Position();
	
	UE_LOG(LogMonster, Warning, TEXT("Monstro Position: x=%i y=%i"), Next->Value->Position().PositionX,
	       Next->Value->Position().PositionY);
	UE_LOG(LogMonster, Warning, TEXT("Player Position: x=%i y=%i"), NewRoom->Position().PositionX,
	       NewRoom->Position().PositionY);

	if (NewRoom->Position().PositionX == Position.PositionX && NewRoom->Position().PositionY == Position.PositionY)
	{
		Player->DecreaseLives(1);
	}

	FVector Location = Next->Value->GetActorLocation();
	Location.Z = 150.0f;

	SetActorLocation(Location);

	Player->UpdatePositions(NewRoom->Position(), Next->Value->Position());
}
