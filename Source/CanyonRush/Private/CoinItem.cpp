// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"

#include "CarCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACoinItem::ACoinItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMwsh"));
	CoinMesh->SetupAttachment(SphereCollider);
	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, 90, 0);
}

// Called when the game starts or when spawned
void ACoinItem::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACoinItem::OnSphereOverlap);
}

void ACoinItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACarCharacter* CarCharacter = Cast<ACarCharacter>(OtherActor);

	if (CarCharacter)
	{
		if (OverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OverlapSound, GetActorLocation());
		}
		
		CarCharacter->AddCoin();

		Destroy();
	}
}
