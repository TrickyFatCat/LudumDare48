#include "Room/Room.h"


#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Color.h"

DEFINE_LOG_CATEGORY_STATIC(LogRoom, All, All)



ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;

	Floor = CreateDefaultSubobject<USceneComponent>(TEXT("Floor"));
	RootComponent = Floor;
	
	for (int i = 0; i < 4; ++i)
	{
		const FName PortalName = FName(TEXT("Portal"), i + 1);
		const FName SpawnName = FName(TEXT("PlayerSpawnPoint"), i + 1);

		Portals.Add(CreateDefaultSubobject<UBoxComponent>(PortalName));
		Portals[i]->SetupAttachment(GetRootComponent());
		Portals[i]->SetHiddenInGame(true);
		// Portals[i]->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Portals[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Portals[i]->SetCollisionObjectType(ECC_WorldDynamic);
		Portals[i]->SetCollisionResponseToChannels(ECR_Ignore);
		Portals[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		

		FVector NewRelativeLocation{FVector::ZeroVector};
		FVector NewExtent{FVector::ZeroVector};

		switch (i)
		{
		case 0:
			NewRelativeLocation = FVector(0.f, -460.f, 50.f);
			NewExtent = FVector(128.f, 32.f, 128.f);
			break;

		case 1:
			NewRelativeLocation = FVector(460.f, 0.f, 50.f);
			NewExtent = FVector(32.f, 128.f, 128.f);
			break;

		case 2:
			NewRelativeLocation = FVector(0.f, 460.f, 50.f);
			NewExtent = FVector(128.f, 32.f, 128.f);
			break;

		case 3:
			NewRelativeLocation = FVector(-460.f, 0.f, 50.f);
			NewExtent = FVector(32.f, 128.f, 128.f);
			break;
		}

		
		Portals[i]->SetRelativeLocation(NewRelativeLocation);
		Portals[i]->SetBoxExtent(NewExtent);

		
		PlayerSpawnPoints.Add(CreateDefaultSubobject<UArrowComponent>(SpawnName));
		PlayerSpawnPoints[i]->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		PlayerSpawnPoints[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		switch (i)
		{
		case 0:
			NewRelativeLocation = FVector(0.f, 340.f, 150.f);
			break;

		case 1:
			NewRelativeLocation = FVector(-340.f, 0.f, 150.f);
			break;

		case 2:
			NewRelativeLocation = FVector(0.f, -340.f, 150.f);
			
			break;

		case 3:
			NewRelativeLocation = FVector(340.f, 0.f, 150.f);
			break;
		}

		PlayerSpawnPoints[i]->SetRelativeLocation(NewRelativeLocation);
		PlayerSpawnPoints[i]->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(NewRelativeLocation, FVector::ZeroVector));
	}
	
	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Camera->SetupAttachment(GetRootComponent());
	// Camera->SetRelativeLocation(FVector(-1600.f, 1600.f, 2280.f));
	// Camera->SetRelativeRotation(FRotator(-50.f, -45.f, 0.f));
}


void ARoom::BeginPlay()
{
	Super::BeginPlay();

	for (auto P : Portals)
	{
		P->OnComponentBeginOverlap.AddDynamic(this, &ARoom::MovePortal);
	}
}

void ARoom::SetPortalDirection(const EPortalDirection Direction, ARoom* Room, const bool IsMainPath = false)
{
	PortalDirection[Direction] = FRoomPortal(Direction, Room, IsMainPath);
}

TArray<FRoomPortal> ARoom::GetPortalDirections()
{
	TArray<FRoomPortal> Result;
	for (auto D : PortalDirection) Result.Add(D);
	return Result;
}


void ARoom::UpdateColor(const FLinearColor Color) const
{
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	if (!Components[0]) return;

	UMaterialInstanceDynamic* DynMaterial = Components[0]->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial) DynMaterial->SetVectorParameterValue("Base Color", Color);
}

void ARoom::MovePortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	UBoxComponent* Portal = Cast<UBoxComponent>(OverlappedComponent);
	if(!PlayerCharacter && !Portal) return;

	int i = 0;
	for (auto P : Portals)
	{
		if (P == Portal) break;
		i++;
	}

	if(i == 4) return;
	if(!PortalDirection[i].Room) return;

	SetActorHiddenInGame(true);
	PortalDirection[i].Room->SetActorHiddenInGame(false);
	OnPlayerTeleported.Broadcast(PortalDirection[i].Room, PlayerCharacter);
	PlayerCharacter->TeleportPlayer(PortalDirection[i].Room->PlayerSpawnPoints[i]->GetComponentLocation());
}
