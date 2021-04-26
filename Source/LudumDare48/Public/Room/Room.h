#pragma once

#include "CoreMinimal.h"

#include <array>

#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "Room.generated.h"

class ARoom;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerTeleported, ARoom*, NewRoom, APlayerCharacter*, Player);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PositionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	FRoomPortal(): Direction(), Room(nullptr) {}
	FRoomPortal(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath)
	{
		this->Direction = Direction;
		this->Room = Room;
		this->IsMainPath = IsMainPath;
	}

	EPortalDirection Direction;
	
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
	std::array<FRoomPortal, 4> PortalDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Root", meta=(AllowPrivateAccess="true"))
	USceneComponent* Floor{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Floor", meta=(AllowPrivateAccess="true"))
	USceneComponent* CameraAnchor{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Portals", meta=(AllowPrivateAccess="true"))
	TArray<UBoxComponent*> Portals{};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerSpawn", meta=(AllowPrivateAccess="true"))
	TArray<UArrowComponent*> PlayerSpawnPoints{};

protected:
	virtual void BeginPlay() override;

public:
	ARoom();

	FRoomPosition Position() const { return RoomPosition; }
	void SetPosition(const FRoomPosition Position) { RoomPosition = Position; }

	FRoomProperties Properties() const { return RoomProperties; }
	void SetProperties(const FRoomProperties Properties) { RoomProperties = Properties; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<FRoomPortal> GetPortalDirections();

	std::array<FRoomPortal, 4> PortalDirections() { return PortalDirection; }
	void SetPortalDirection(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath);

	void UpdateColor(FLinearColor Color) const;
	
	UFUNCTION()
	void MovePortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerTeleported OnPlayerTeleported;
};
