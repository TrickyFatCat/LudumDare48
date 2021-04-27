#include "Room/RoomsHub.h"

#include "Monster.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogRoomsHub, All, All)

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
	Grid.Init(*new TArray<FNode*>(), Rows);
	Graph = nullptr;

	StartRoom = nullptr;
	const int32 StartX = FMath::RandRange(0, Rows - 1);
	const int32 StartY = FMath::RandRange(0, Rows - 1);

	FNode* Goal = nullptr;
	int32 GoalX = FMath::RandRange(0, Rows - 1);
	int32 GoalY = FMath::RandRange(0, Rows - 1);
	while (
		std::abs(StartX - GoalX) < MinimalDistanceBetweenStartEnd &&
		std::abs(StartY - GoalY) < MinimalDistanceBetweenStartEnd
	)
	{
		GoalX = FMath::RandRange(0, Rows - 1);
		GoalY = FMath::RandRange(0, Rows - 1);
	}

	int32 MonsterX = FMath::RandRange(0, Rows - 1);
	int32 MonsterY = FMath::RandRange(0, Rows - 1);
	while (
		std::abs(StartX - MonsterX) < MinimalDistanceBetweenStartEnd &&
		std::abs(StartY - MonsterY) < MinimalDistanceBetweenStartEnd
	)
	{
		MonsterX = FMath::RandRange(0, Rows - 1);
		MonsterY = FMath::RandRange(0, Rows - 1);
	}

	int32 KeyRoom1X = FMath::RandRange(0, Rows - 1);
	int32 KeyRoom1Y = FMath::RandRange(0, Rows - 1);
	while (
		(std::abs(StartX - KeyRoom1X) < 1 && std::abs(StartY - KeyRoom1Y) < 1) ||
		(std::abs(GoalX - KeyRoom1X) < 1 && std::abs(GoalY - KeyRoom1Y) < 1)
	)
	{
		KeyRoom1X = FMath::RandRange(0, Rows - 1);
		KeyRoom1Y = FMath::RandRange(0, Rows - 1);
	}
	
	int32 KeyRoom2X = FMath::RandRange(0, Rows - 1);
	int32 KeyRoom2Y = FMath::RandRange(0, Rows - 1);
	while (
		(std::abs(StartX - KeyRoom2X) < 1 && std::abs(StartY - KeyRoom2Y) < 1) ||
		(std::abs(GoalX - KeyRoom2X) < 1 && std::abs(GoalY - KeyRoom2Y) < 1) ||
		(std::abs(KeyRoom1X - KeyRoom2X) < 1 && std::abs(KeyRoom1Y - KeyRoom2Y) < 1)
	)
	{
		KeyRoom2X = FMath::RandRange(0, Rows - 1);
		KeyRoom2Y = FMath::RandRange(0, Rows - 1);
	}
	
	int32 KeyRoom3X = FMath::RandRange(0, Rows - 1);
	int32 KeyRoom3Y = FMath::RandRange(0, Rows - 1);
	while (
	(std::abs(StartX - KeyRoom3X) < 1 && std::abs(StartY - KeyRoom3Y) < 1) ||
	(std::abs(GoalX - KeyRoom3X) < 1 && std::abs(GoalY - KeyRoom3Y) < 1) ||
	(std::abs(KeyRoom1X - KeyRoom3X) < 1 && std::abs(KeyRoom1Y - KeyRoom3Y) < 1) ||
	(std::abs(KeyRoom2X - KeyRoom3X) < 1 && std::abs(KeyRoom2Y - KeyRoom3Y) < 1)
	)
	{
		KeyRoom3X = FMath::RandRange(0, Rows - 1);
		KeyRoom3Y = FMath::RandRange(0, Rows - 1);
	}

	Monster = World->SpawnActorDeferred<AMonster>(
			MonsterClass,
			FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	if (!Monster) return;

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			TSubclassOf<AActor> RoomClass =
				FMath::FRandRange(0, 1) <= ObstacleHazard
					? ObstacleRoomClass
					: DefaultRoomClasses[FMath::RandRange(0, DefaultRoomClasses.Num() - 1)];

			if (i == StartX && j == StartY) RoomClass = StartRoomClass;
			if (i == GoalX && j == GoalY) RoomClass = EndRoomClass;

			if (i == KeyRoom1X && j == KeyRoom1Y) RoomClass = KeyRooms[0];
			if (i == KeyRoom2X && j == KeyRoom2Y) RoomClass = KeyRooms[1];
			if (i == KeyRoom3X && j == KeyRoom3Y) RoomClass = KeyRooms[2];

			ARoom* Room = World->SpawnActorDeferred<ARoom>(
				RoomClass,
				FTransform(FRotator::ZeroRotator, FVector::ZeroVector)
			);
			if (!Room) continue;

			FTransform Position = FTransform(
				FRotator::ZeroRotator,
				FVector(100.0f * j * Scale, 100.0f * i * Scale, 50.0f)
			);
			Room->SetProperties(FRoomProperties(RoomClass == ObstacleRoomClass ? true : false));
			Room->SetPosition(FRoomPosition(i, j));
			Room->SetActorHiddenInGame(RoomClass != StartRoomClass ? true : false);
			Room->FinishSpawning(Position);
			Room->OnPlayerTeleported.AddDynamic(Monster, &AMonster::OnPlayerTeleported);

			Grid[i].Add(new FNode(Room));

			// Graph == nullptr ? Graph = new FGraph(Grid[0][0]) : Graph->AddNode(Grid[i][j]);

			if (Graph == nullptr)
			{
				Graph = new FGraph(Grid[0][0]);
			}
			else
			{
				Graph->AddNode(Grid[i][j]);
			}
			if (RoomClass == StartRoomClass) StartRoom = Grid[i][j];
			if (RoomClass == EndRoomClass) Goal = Grid[i][j];
		}
	}

	CreateLinks();
	const std::deque<FNode*> Path = Graph->PathBfs(Grid[StartX][StartY], Grid[GoalX][GoalY]);
	StartRoom = Grid[StartX][StartY];
	Goal = Grid[GoalX][GoalY];
	UpdateMainPath(Path);

		const FTransform PositionMonster = FTransform(
			FRotator::ZeroRotator,
			FVector(100.0f * MonsterY * Scale, 100.0f * MonsterX * Scale, 300.0f)
		);
		Monster->Position = FRoomPosition(MonsterX, MonsterY);
		Monster->Graph = Graph;
		Monster->Grid = Grid;
		Monster->FinishSpawning(PositionMonster);

		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player)
		{
			Player->UpdatePositions(Grid[StartX][StartY]->Value->Position(), Monster->Position);
			Player->OnMoveToStart.AddDynamic(this, &ARoomsHub::OnRestartRoom);
		}
		
	
}

inline void ARoomsHub::OnRestartRoom()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Grid[Player->Position.PositionX][Player->Position.PositionY]->Value->SetActorHiddenInGame(true);
	StartRoom->Value->SetActorHiddenInGame(false);
	Player->UpdatePositions(StartRoom->Value->Position(), Monster ? Monster->Position : FRoomPosition(0,0));
}


void ARoomsHub::CreateLinks() const
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			if ((i - 1) > 0)
			{
				Graph->Connect(Grid[i - 1][j], Grid[i][j]);
				if (!Grid[i - 1][j]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(North, Grid[i - 1][j]->Value, false);
			}
			else if (!Grid[Rows - 1][j]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(North, Grid[Rows - 1][j]->Value, false);

			if (i + 1 < Rows)
			{
				Graph->Connect(Grid[i + 1][j], Grid[i][j]);
				if (!Grid[i + 1][j]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(South, Grid[i + 1][j]->Value, false);
			}
			else if (!Grid[0][j]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(South, Grid[0][j]->Value, false);

			if (j - 1 > 0)
			{
				Graph->Connect(Grid[i][j - 1], Grid[i][j]);
				if (!Grid[i][j - 1]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(West, Grid[i][j - 1]->Value, false);
			}
			else if (!Grid[i][Rows - 1]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(West, Grid[i][Rows - 1]->Value, false);


			if (j + 1 < Rows)
			{
				Graph->Connect(Grid[i][j + 1], Grid[i][j]);
				if (!Grid[i][j + 1]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(East, Grid[i][j + 1]->Value, false);
			}
			else if (!Grid[i][0]->IsObstacle) Grid[i][j]->Value->SetPortalDirection(East, Grid[i][0]->Value, false);
		}
	}
}

void ARoomsHub::UpdateMainPath(std::deque<FNode*> Path) const
{
	ARoom* Last = nullptr;
	for (auto N : Path)
	{
		if (IsRenderPath && *Path.begin() != N) N->Value->UpdateColor(FLinearColor::Yellow);

		FRoomProperties c = N->Value->Properties();
		c.IsMainPath = true;
		N->Value->SetProperties(c);

		if (Last)
		{
			for(const auto P : Last->PortalDirections())
			{
				if (P.Room != N->Value) continue;
				Last->SetPortalDirection(P.Direction, P.Room, true);
			}

			for(const auto P : N->Value->PortalDirections())
			{
				if (P.Room != Last) continue;
				N->Value->SetPortalDirection(P.Direction, P.Room, true);
			}	
		}

		Last = N->Value;
	}
}

TArray<ARoom*> ARoomsHub::PathBetween(const FRoomPosition Start, const FRoomPosition End)
{
	const std::deque<FNode*> Path = Graph->PathBfs(
		Grid[Start.PositionX][Start.PositionY],
		Grid[End.PositionX][End.PositionY]
		);
	TArray<ARoom*> Array;
	for (auto P : Path) Array.Add(P->Value);
	
	return Array;
}

void ARoomsHub::ShufflePortals()
{
	for(auto Line : Grid)
	{
		for(auto Node : Line)
		{
			
		}
	}
}
