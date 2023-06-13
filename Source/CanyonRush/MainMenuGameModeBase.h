// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CANYONRUSH_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;
};
