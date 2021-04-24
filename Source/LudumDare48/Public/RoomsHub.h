#pragma once

#include "CoreMinimal.h"

#include "Graph.h"
#include "GameFramework/Actor.h"
#include "RoomsHub.generated.h"

UCLASS()
class LUDUMDARE48_API ARoomsHub : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomsHub();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	int Rows = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	int Scale = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	TSubclassOf<ARoom> StartRoomClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	TSubclassOf<ARoom> EndRoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	TArray<TSubclassOf<ARoom>> DefaultRoomClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	float ObstacleHazard = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	TSubclassOf<ARoom> ObstacleRoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generate")
	bool IsRenderPath;


protected:
	virtual void BeginPlay() override;
	void GenerateRooms();
	void CreateLinks(FGraph* Graph, TArray<TArray<FNode*>> Grid) const;
	
	void TestUpdateColor(std::deque<FNode*> Path);
};
