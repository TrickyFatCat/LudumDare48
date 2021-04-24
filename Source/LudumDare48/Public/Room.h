#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UENUM(BlueprintType)
 enum class ERoomType : uint8 {
      Free = 0	UMETA(DisplayName = "DEFAULT"),
      Start = 1	UMETA(DisplayName = "START"),
      End = 2	UMETA(DisplayName = "END"),
 };

UCLASS()
class LUDUMDARE48_API ARoom : public AActor
{
	GENERATED_BODY()
	
private:	
	int PositionX;
	int PositionY;

protected:
	virtual void BeginPlay() override;

public:	
	ARoom();
	
	virtual void Tick(float DeltaTime) override;
	void UpdateColor(FLinearColor Color) const;

	ERoomType Type = ERoomType::Free;
	bool IsObstacle = false;
};
