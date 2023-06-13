// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameModeBase.h"

#include "Blueprint/UserWidget.h"

void AMainMenuGameModeBase::BeginPlay()
{
	if (IsValid(MainMenuWidgetClass))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), MainMenuWidgetClass);

		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
