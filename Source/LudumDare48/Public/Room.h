#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class ARoom;

enum EPortalDirection
{
	North = 0,
	East = 1,
	South = 2,
	West = 3,
};

USTRUCT(BlueprintType)
struct FRoomPosition
{
	GENERATED_USTRUCT_BODY()

	FRoomPosition(): PositionX(0), PositionY(0) {}
	FRoomPosition(const int X, const int Y)
	{
		PositionX = X;
		PositionY = Y;
	};

	UPROPERTY(EditAnywhere)
	int PositionX;

	UPROPERTY(EditAnywhere)
	int PositionY;
};

USTRUCT(BlueprintType)
struct FRoomProperties
{
	GENERATED_USTRUCT_BODY()

	FRoomProperties() {};
	FRoomProperties(const bool IsObstacle) { this->IsObstacle = IsObstacle; };

	UPROPERTY(EditAnywhere)
	bool IsObstacle = false;

	UPROPERTY(EditAnywhere)
	bool IsMainPath = false;
};

USTRUCT(BlueprintType)
struct FRoomPortal
{
	GENERATED_USTRUCT_BODY()

	FRoomPortal(): Room(nullptr) {}
	FRoomPortal(ARoom* Room, const bool IsMainPath)
	{
		this->Room = Room;
		this->IsMainPath = IsMainPath;
	};

	UPROPERTY(EditAnywhere)
	ARoom* Room;

	UPROPERTY(EditAnywhere)
	bool IsMainPath = false;
};

UCLASS()
class LUDUMDARE48_API ARoom : public AActor
{
	GENERATED_BODY()

private:
	FRoomPosition RoomPosition;
	FRoomProperties RoomProperties;
	FRoomPortal* PortalDirection[4];

protected:
	virtual void BeginPlay() override;

public:
	ARoom();

	FRoomPosition Position() const { return RoomPosition; }
	void SetPosition(const FRoomPosition Position) { RoomPosition = Position; }

	FRoomProperties Properties() const { return RoomProperties; }
	void SetProperties(const FRoomProperties Properties) { RoomProperties = Properties; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<FRoomPortal> GetPortalDirection();

	void SetPortalDirection(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath);

	void UpdateColor(FLinearColor Color) const;
};
