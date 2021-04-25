#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UENUM(BlueprintType)
 enum class EPortalDirection: uint8 {
      North = 0	UMETA(DisplayName = "North"),
      East = 1	UMETA(DisplayName = "East"),
      South = 2	UMETA(DisplayName = "South"),
      West = 2	UMETA(DisplayName = "West"),
 };

UCLASS()
class LUDUMDARE48_API ARoom : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	ARoom();
	
	virtual void Tick(float DeltaTime) override;
	void UpdateColor(FLinearColor Color) const;
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<ARoom*> GetPortalDirection();

	ARoom* PortalDirection[4];
	
	bool IsObstacle = false;
};
