// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "CarCharacter.generated.h"

UCLASS()
class CANYONRUSH_API ACarCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleInstanceOnly)
	class ACanyonRushGameModeBase* RunGameMode;
	
public:
	// Sets default values for this character's properties
	ACarCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")
	class UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")
	class USoundBase* DeathSound;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	UFUNCTION(BlueprintImplementableEvent, Category="Lane")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category="Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category="Lane")
	void ChangeLaneFinished();

	UFUNCTION(BlueprintCallable)
	void Death();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UCameraComponent *CameraComponent;
	
	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void ResetLevel();

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UPROPERTY()
	class APlayerStart* PlayerStart;

	UPROPERTY()
	bool bIsDead = false;
	
	virtual void BeginPlay() override;


public:

	UFUNCTION()
	void AddCoin();
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
