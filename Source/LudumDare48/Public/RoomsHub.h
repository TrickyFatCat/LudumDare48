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

protected:
	virtual void BeginPlay() override;
	
	FGraph* Graph;
	FNode* Start;
	FNode* Goal;

	void GenerateRooms();
};
