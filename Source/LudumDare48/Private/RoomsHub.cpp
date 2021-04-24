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
	FNode* Grid[Rows][Rows];
	
	if (Graph != nullptr) {
		delete Graph;
		Graph = nullptr;
	}
}
