// Fill out your copyright notice in the Description page of Project Settings.

#include "WhyGameMode.h"

bool AWhyGameMode::bInGameMenu = true;

AWhyGameMode::AWhyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("Class'/Script/Why.WhyPawn'"));
	if (PlayerPawnClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AWhyPlayerController> BPPlayerControllerClass(TEXT("Blueprint'/Game/Blueprint/BP_WhyPlayerController.BP_WhyPlayerController_C'"));
	if (BPPlayerControllerClass.Class != NULL)
	{
		PlayerControllerClass = BPPlayerControllerClass.Class;
	}

	GameStateClass = AWhyGameState::StaticClass();
	

	PlayerStateClass = AWhyPlayerState::StaticClass();
	HUDClass = AWhyHUD::StaticClass();

	bReplicates = true;
}

void AWhyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		APlayerController* thisCont = GetWorld()->GetFirstPlayerController();
		if (thisCont)
		{
			AWhyPawn* thisPawn = Cast<AWhyPawn>(thisCont->GetPawn());
			Spawn(thisPawn);
		}
		Cast<AWhyGameState>(GameState)->bInMenu = bInGameMenu;
	}

}

void AWhyGameMode::Tick(float DeltaSeconds)
{
	if (Role == ROLE_Authority)
	{
		APlayerController* thisCont = GetWorld()->GetFirstPlayerController();
		if (ToBeSpawned.Num() != 0)
		{
			for (auto pawnToSpawn : ToBeSpawned)
			{
				Spawn(pawnToSpawn);
			}
		}
		if (thisCont != nullptr&&thisCont->IsInputKeyDown(EKeys::R))
		{
			bInGameMenu = false;
			GetWorld()->ServerTravel(L"/Game/Map/PlayMap.PlayMap?Listen");
			Cast<AWhyGameState>(GameState)->bInMenu = bInGameMenu;
		}
	}
}


void AWhyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	if (NewPlayer)
	{
		AWhyPlayerState* PS = Cast<AWhyPlayerState>(NewPlayer->PlayerState);
		AWhyGameState* GS = Cast<AWhyGameState>(GameState);
		AWhyPawn* Pawn = Cast<AWhyPawn>(NewPlayer->GetPawn());
		
		PlayerList.Add(NewPlayer);

		int32 Number=1;
		
		//입장한 플레이어들에 대하여
		for (APlayerState* It : GameState->PlayerArray)
		{
			AWhyPlayerState* OtherPS = Cast<AWhyPlayerState>(It);
			if (OtherPS)
			{
				//플레이어들에게 이름과 번호를 배정
				PS->SetPlayerNumber(Number);
				SelectNameNumber = FMath::Rand() % PlayerNameArray.Num();
				PS->SetPlayerRandomName(PlayerNameArray[SelectNameNumber]);
				Pawn->SetShowPlayerNameText(PS->GetPlayerRandomName());
				//GS->SetPlayersName(PlayerNameArray[SelectNameNumber], PS->GetPlayerNumber());
				
				
				Number++;
				PS->SetTargetNumber();
			}

		}
		//GEngine->AddOnScreenDebugMessage(-1, 5000.f, FColor::Red, FString::Printf(TEXT("Number %d"),SelectNameNumber));
		PlayerNameArray.RemoveAt(SelectNameNumber);
		
		GS->SetInPlayerCount(GS->GetInPlayerCount() + 1);

		Spawn(Pawn);
	}
	
}




TArray<APlayerController*> AWhyGameMode::GetPlayerList()
{
	return PlayerList;
}

void AWhyGameMode::Spawn(class AWhyPawn* Pawn)
{
	if (Role == ROLE_Authority)
	{
		for (int32 i = 1; i <= 5; i++)
		{
			Pawn->SetActorLocation(FVector(1000.0f, 940.0f, 3500.0f));
			return;
		}
		
	}
	if (ToBeSpawned.Find(Pawn) == INDEX_NONE)
	{
		ToBeSpawned.Add(Pawn);
	}
}






