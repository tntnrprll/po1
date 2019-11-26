#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "ColorBox.h"
#include "MyActor.generated.h"

UCLASS()
class WHY_API AMyActor : public AActor
{
	GENERATED_BODY()
	
protected:
	AMyActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable,Category=Grid)
	void GenerateGrid(int InGridWidth, int InGridHeight, int InTileSize);

	UFUNCTION(BlueprintPure, Category = Grid)
		FVector ConvertGridIndexToWorldLocationA(FVector2D InGridIndex);
	UFUNCTION(BlueprintPure, Category = Grid)
		FVector2D ConvertWorldIndexToWorldLocation(FVector InWorldLocation);
	UFUNCTION(BlueprintPure, Category = Grid)
		FVector ConvertGridIndexToWorldLocationB(FVector2D InGridIndex);
	UFUNCTION(BlueprintPure, Category = Grid)
		FVector ConvertGridIndexToWorldLocationC(FVector2D InGridIndex);
	UPROPERTY(EditAnywhere, Category = Value)
		int32 Value=1;
	UPROPERTY(VisibleAnywhere)
		FVector Location;

public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Scene)
	USceneComponent* GridRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scene)
	TMap<FVector2D, UStaticMeshComponent*> GridMap;

	UPROPERTY(VisibleAnywhere)
		TArray<FVector2D> GridArray;
	UPROPERTY(VisibleAnywhere)
	int GridWidth=4;
	UPROPERTY(VisibleAnywhere)
	int GridHeight=4;
	int TileSize=100;

	UPROPERTY(EditAnywhere, Category = Rotation)
		FRotator Rotation;

	UPROPERTY(EditAnywhere, Category = SpawnObject)
	TSubclassOf<AColorBox> ColorBox;
	UFUNCTION()
	void SpawnColorBox();
	
};
