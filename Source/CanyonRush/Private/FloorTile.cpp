// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Obstacle.h"
#include "CanyonRush/CanyonRushGameModeBase.h"
#include "CarCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	CentreLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Centre Lane"));
	AttachPoint->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	AttachPoint->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	AttachPoint->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ACanyonRushGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACarCharacter* CarCharacter = Cast<ACarCharacter>(OtherActor);

	if (CarCharacter)
	{
		RunGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 2.0f, false);
	}
}

void AFloorTile::SpawnItems()
{
	if(IsValid(FirstObstacleClass))
	{
		SpawnLaneItem(CentreLane);
		SpawnLaneItem(LeftLane);
		SpawnLaneItem(RightLane);
	}

}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane)
{
	const float RandVal = FMath::FRandRange(0.0f, 1.0f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();
	
	if (UKismetMathLibrary::InRange_FloatFloat(RandVal, 0.5f, 1.0f, true, true))
	{
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(FirstObstacleClass, SpawnLocation, SpawnParameters);
		ChildActors.Add(Obstacle);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	for (auto Child : ChildActors)
	{
		if (IsValid(Child))
		{
			Child->Destroy();
		}
	}

	ChildActors.Empty();

	RunGameMode->RemoveTile(this);
	
	this->Destroy();
}