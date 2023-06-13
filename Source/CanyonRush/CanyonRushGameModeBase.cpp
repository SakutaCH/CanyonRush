// Copyright Epic Games, Inc. All Rights Reserved.


#include "CanyonRushGameModeBase.h"

#include "FloorTile.h"
#include "GameHUD.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

void ACanyonRushGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(),0)->bShowMouseCursor = true;

	GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), GameHUDClass));
	check(GameHUD);

	GameHUD->InitializeHUD(this);
	GameHUD->AddToViewport();

	NumberOfLives = MaxLives;
	
	CreateInitialFloorTiles();
}

void ACanyonRushGameModeBase::CreateInitialFloorTiles()
{
	AFloorTile* Tile = AddFloorTile(false);

	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CentreLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	// for (float Val : LaneSwitchValues)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Lane Value: %f"), Val);
	// }

	AddFloorTile(false);
	AddFloorTile(false);
	
	for(int i = 0; i < NumInitialFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* ACanyonRushGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();

	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if(Tile)
		{
			FloorTiles.Add(Tile);
			
			if(bSpawnItems)
			{
				Tile->SpawnItems();
			}
			
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return  Tile;
	}
	return nullptr;
}

void ACanyonRushGameModeBase::AddCoin()
{
	TotalCoins += 1;

	OnCoinsCountChanged.Broadcast(TotalCoins);
}

void ACanyonRushGameModeBase::PlayerDied()
{
	NumberOfLives -= 1;
	OnLivesCountChanged.Broadcast(NumberOfLives);

	if (NumberOfLives > 0)
	{
		for (auto Tile : FloorTiles)
		{
			// Tile->DestroyFloorTile;
		}

		FloorTiles.Empty();

		NextSpawnPoint = FTransform();

		CreateInitialFloorTiles();

		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void ACanyonRushGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}

void ACanyonRushGameModeBase::GameOver()
{
	if (IsValid(GameOverScreenClass))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), GameOverScreenClass);

		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
