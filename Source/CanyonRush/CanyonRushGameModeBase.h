// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CanyonRushGameModeBase.generated.h"

class AFloorTile;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChanged, int32, CoinsCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesCountChanged, int32, LivesCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelReset);

UCLASS()
class CANYONRUSH_API ACanyonRushGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<UUserWidget> GameHUDClass;

	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<UUserWidget> GameOverScreenClass;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	class UGameHUD* GameHUD;
	
	UPROPERTY(VisibleAnywhere)
	int32 TotalCoins = 0;

	UPROPERTY(VisibleAnywhere)
	int32 NumberOfLives = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxLives = 3;
	
	UPROPERTY(EditAnywhere, Category="Config")
	int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	TArray<float> LaneSwitchValues;

	UPROPERTY(VisibleInstanceOnly,Category="Runtime")
	TArray<AFloorTile*> FloorTiles;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnCoinsCountChanged OnCoinsCountChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnLivesCountChanged OnLivesCountChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnLevelReset OnLevelReset;

	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTile* AddFloorTile(const bool bSpawnItems);

	UFUNCTION(BlueprintCallable)
	void AddCoin();

	UFUNCTION(BlueprintCallable)
	void PlayerDied();

	UFUNCTION(BlueprintCallable)
	void RemoveTile(AFloorTile* Tile);

	UFUNCTION(BlueprintCallable)
	void GameOver();

protected:
	virtual void BeginPlay() override;
};
