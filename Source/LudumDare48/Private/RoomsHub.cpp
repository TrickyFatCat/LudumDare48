#include "RoomsHub.h"

ARoomsHub::ARoomsHub()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ARoomsHub::BeginPlay()
{
	Super::BeginPlay();
	GenerateRooms();
}


void ARoomsHub::GenerateRooms()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<TArray<FNode*>> Grid;
	Grid.Init(*new TArray<FNode*>(), Rows);

	FGraph* Graph = nullptr;

	FNode* Start = nullptr;
	const int32 StartX = FMath::RandRange(0, Rows - 1);
	const int32 StartY = FMath::RandRange(0, Rows - 1);

	FNode* Goal = nullptr;
	int32 GoalX = FMath::RandRange(0, Rows - 1);
	int32 GoalY = FMath::RandRange(0, Rows - 1);
	while (StartX == GoalX && StartY == GoalY)
	{
		GoalX = FMath::RandRange(0, Rows - 1);
		GoalY = FMath::RandRange(0, Rows - 1);
	}

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			TSubclassOf<AActor> RoomClass =
				FMath::FRandRange(0, 1) <= ObstacleHazard
					? ObstacleRoomClass
					: DefaultRoomClasses[FMath::RandRange(0, DefaultRoomClasses.Num() - 1)];

			if (i == StartX && j == StartY)
			{
				RoomClass = StartRoomClass;
			}
			if (i == GoalX && j == GoalY)
			{
				RoomClass = EndRoomClass;
			}

			ARoom* Room = World->SpawnActorDeferred<ARoom>(
				RoomClass,
				FTransform(FRotator::ZeroRotator, FVector::ZeroVector)
			);
			if (!Room) continue;

			FTransform Position = FTransform(
				FRotator::ZeroRotator,
				FVector(100.0f * j * Scale, 100.0f * i * Scale, 0.0f)
			);
			Room->IsObstacle = RoomClass == ObstacleRoomClass ? true : false;
			Room->FinishSpawning(Position);

			Grid[i].Add(new FNode(Room));

			Graph == nullptr ? Graph = new FGraph(Grid[0][0]) : Graph->AddNode(Grid[i][j]);

			if (RoomClass == StartRoomClass)
			{
				Start = Grid[i][j];
			}
			if (RoomClass == EndRoomClass)
			{
				Goal = Grid[i][j];
			}
		}
	}

	CreateLinks(Graph, Grid);
	const std::deque<FNode*> Path = Graph->PathBfs(Start, Goal);

	if (IsRenderPath)
	{
		TestUpdateColor(Path);
	}
}

void ARoomsHub::CreateLinks(FGraph* Graph, TArray<TArray<FNode*>> Grid) const
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			if ((i - 1) > 0)
				Graph->Connect(Grid[i - 1][j], Grid[i][j]);
			if (i + 1 < Rows)
				Graph->Connect(Grid[i + 1][j], Grid[i][j]);
			if (j - 1 > 0)
				Graph->Connect(Grid[i][j - 1], Grid[i][j]);
			if (j + 1 < Rows)
				Graph->Connect(Grid[i][j + 1], Grid[i][j]);
		}
	}
}

void ARoomsHub::TestUpdateColor(std::deque<FNode*> Path)
{
	for (auto N : Path)
	{
		if (*Path.begin() != N)
		{
			N->Value->UpdateColor(FLinearColor::Yellow);
		}
	}
}
