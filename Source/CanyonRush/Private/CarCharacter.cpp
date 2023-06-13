// Fill out your copyright notice in the Description page of Project Settings.


#include "CarCharacter.h"
#include "Camera/CameraComponent.h"
#include "CanyonRush/CanyonRushGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACarCharacter::ACarCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	SpringArmComponent->TargetArmLength = 350.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACarCharacter::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ACanyonRushGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	RunGameMode->OnLevelReset.AddDynamic(this, &ACarCharacter::ResetLevel);

	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
}

void ACarCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}

// Called every frame
void ACarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	
	AddMovementInput(ControlRot.Vector());
}

// Called to bind functionality to input
void ACarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ACarCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ACarCharacter::MoveRight);
}

void ACarCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ACarCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ACarCharacter::Death()
{
	if (!bIsDead)
	{
		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (World)
		{
			bIsDead = true;
			DisableInput(nullptr);
		
			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}

			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}

			GetMesh()->SetVisibility(false);

			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ACarCharacter::OnDeath, 1.0f);
		}
	}
}

void ACarCharacter::OnDeath()
{
	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}
	
	RunGameMode->PlayerDied();
}

void ACarCharacter::ResetLevel()
{
	bIsDead = false;
	EnableInput(nullptr);
	GetMesh()->SetVisibility(true);
	
	if (PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}
}

void ACarCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ACarCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

