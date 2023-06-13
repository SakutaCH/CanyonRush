// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class AObstacle;
class ACoinItem;
class UArrowComponent;

UCLASS()
class CANYONRUSH_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<AObstacle> FirstObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<ACoinItem> CoinItemClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* CentreLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ChildActors;

	UFUNCTION(BlueprintCallable)
	void SpawnItems();
	
	UFUNCTION(BlueprintCallable)
	void DestroyFloorTile();
	
	AFloorTile();

	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}
	
	
protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleInstanceOnly)
	class ACanyonRushGameModeBase* RunGameMode;

	UPROPERTY()
	FTimerHandle DestroyHandle;

	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void SpawnLaneItem(UArrowComponent* Lane);
	
	virtual void BeginPlay() override;
};
