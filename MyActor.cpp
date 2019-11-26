#include "MyActor.h"


AMyActor::AMyActor()
{
 	PrimaryActorTick.bCanEverTick = true;
	GridRootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("GridRootComponent"));
	RootComponent = GridRootComponent;

	bReplicates = true;
}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateGrid(10,10,325);
	SpawnColorBox();
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AMyActor::GenerateGrid(int InGridWidth, int InGridHeight, int InTileSize)
{
	GridMap.Empty();
	GridWidth = InGridWidth;
	GridHeight = InGridHeight;
	TileSize = InTileSize;
	for (int X = 0; X < InGridWidth; X++)
	{
		for (int Y = 0; Y < InGridHeight; Y++)
		{
			GridMap.Add(FVector2D(X, Y),nullptr);
			GridArray.Add(FVector2D(X, Y));
		}
	}
}

FVector AMyActor::ConvertGridIndexToWorldLocationA(FVector2D InGridIndex)
{
	FVector GridLocation = GetActorLocation();
	float X=GridLocation.X+InGridIndex.X*TileSize;
	float Y=GridLocation.Y+InGridIndex.Y*TileSize;
	float Z = GridLocation.Z;

	return FVector(X,Y,Z);

}
FVector AMyActor::ConvertGridIndexToWorldLocationB(FVector2D InGridIndex)
{
	FVector GridLocation = GetActorLocation();
	float X = GridLocation.X;
	float Y = GridLocation.Y + InGridIndex.X*TileSize;
	float Z = GridLocation.Z + InGridIndex.Y*TileSize;

	return FVector(X, Y, Z);

}

FVector AMyActor::ConvertGridIndexToWorldLocationC(FVector2D InGridIndex)
{
	FVector GridLocation = GetActorLocation();
	float X = GridLocation.X + InGridIndex.X*TileSize;
	float Y = GridLocation.Y ;
	float Z = GridLocation.Z + InGridIndex.Y*TileSize;

	return FVector(X, Y, Z);

}


FVector2D AMyActor::ConvertWorldIndexToWorldLocation(FVector InWorldLocation)
{
	FVector GridLocation = GetActorLocation();

	int X=FMath::RoundToInt((InWorldLocation.Y-GridLocation.Y)/TileSize);
	int Y= FMath::RoundToInt((InWorldLocation.Z - GridLocation.Z) / TileSize);

	return FVector2D(X,Y);
}

void AMyActor::SpawnColorBox()
{
	if (Role == ROLE_Authority)
	{
		for (auto GridMapNum = GridMap.CreateIterator(); GridMapNum; ++GridMapNum)
		{
			FActorSpawnParameters SpawnParameter;

			if (Value == 1)
			{
				Location = ConvertGridIndexToWorldLocationA(GridMapNum.Key());
			}
			else if (Value == 2)
			{
				Location = ConvertGridIndexToWorldLocationB(GridMapNum.Key());
			}
			else if (Value == 3)
			{
				Location = ConvertGridIndexToWorldLocationC(GridMapNum.Key());
			}


			GetWorld()->SpawnActor<AActor>(ColorBox, Location, Rotation, SpawnParameter);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("location X: %f Y: %f Z: %f"), Location.X,Location.Y,Location.Z));
		}
	}
	
}



