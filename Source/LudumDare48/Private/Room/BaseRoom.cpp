// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/BaseRoom.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseRoom::ABaseRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Floor = CreateDefaultSubobject<USceneComponent>(TEXT("Floor"));
	RootComponent = Floor;

	CameraAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("CameraAnchor"));
	CameraAnchor->SetupAttachment(Floor);

    for (int i = 0; i < 4; ++i)
    {
	    const FName ComponentName = FName(TEXT("PlayerEnterPoint"), i + 1);
		PlayerSpawnPoints.Add(CreateDefaultSubobject<UArrowComponent>(ComponentName));
    	UArrowComponent* PlayerEnterPoint = PlayerSpawnPoints[i];
    	FVector NewRelativeLocation{FVector::ZeroVector};

    	switch (i)
    	{
    		case 0:
    			NewRelativeLocation = FVector(0.f, -600.f, 50.f);
    			break;

    		case 1:
    			NewRelativeLocation = FVector(600.f, 0.f, 50.f);
    			break;

    		case 2:
    			NewRelativeLocation = FVector(0.f, 600.f, 50.f);
    			break;

    		case 3:
    			NewRelativeLocation = FVector(-600.f, 0.f, 50.f);
    			break;
    	}
    	
		PlayerEnterPoint->SetRelativeLocation(NewRelativeLocation);
		PlayerEnterPoint->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(NewRelativeLocation, FVector::ZeroVector));
    }

	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Camera->SetupAttachment(GetRootComponent());
	// Camera->SetRelativeLocation(FVector(-1600.f, 1600.f, 2280.f));
	// Camera->SetRelativeRotation(FRotator(-50.f, -45.f, 0.f));
}

// Called when the game starts or when spawned
void ABaseRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}