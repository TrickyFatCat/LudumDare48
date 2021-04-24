#include "Room.h"

// Sets default values
ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoom::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
