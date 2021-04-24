#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UENUM(BlueprintType)
 enum class ERoomType : uint8 {
      Free = 0	UMETA(DisplayName = "FREE"),
      Start = 1	UMETA(DisplayName = "START"),
      End = 2	UMETA(DisplayName = "END"),
 };

UCLASS()
class LUDUMDARE48_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ERoomType Type = ERoomType::Free;
	bool IsObstacle = false;
};
