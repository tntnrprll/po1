// Fill out your copyright notice in the Description page of Project Settings.



#include "WhyPlayerController.h"
#include "Engine/World.h"

AWhyPlayerController::AWhyPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AWhyPlayerController::BeginPlay()
{
	Super::BeginPlay();

}
void AWhyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AWhyPlayerController::InputClickPressed()
{
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);

	if (State)
	{
		if (!WhatTime&&!State->GetIsDie())
		{
			MoveToMouseCursor();
		}
	}
	
	
}

void AWhyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RightClick", IE_Pressed, this, &AWhyPlayerController::InputClickPressed);
	InputComponent->BindAction("InputClick", IE_Pressed, this, &AWhyPlayerController::LeftClickPressed);


}

void AWhyPlayerController::LeftClickPressed()
{
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);

	if (State)
	{

		if (WhatTime&&!State->GetIsDie())
		{

			AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
			AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());

			FHitResult Hit;
			GetHitResultUnderCursor(ECC_Visibility, false, Hit);
			if (Hit.bBlockingHit)
			{
				AColorBox* HitActor = Cast<AColorBox>(Hit.Actor);

				if (HitActor)
				{
					if (PS->GetUpCount() - GS->GetChangeCost() >= 0)
					{
						Clicked(HitActor);
						PS->SetUpCount(PS->GetUpCount() - GS->GetChangeCost());
						ServerAddChangeCost();
					}
					else
					{
						int32 chargecost = GS->GetChangeCost() - PS->GetUpCount();
						if (PS->GetPlayerScore() >= chargecost)
						{
							Clicked(HitActor);
							PS->SetUpCount(0);
							ServerSetPlayerScore(-chargecost);
							ServerAddChangeCost();
						}
					}
				}
			}
		}
	}


}

void AWhyPlayerController::Clicked(AColorBox* Box)
{
	ServerSetClickedNumber(Box);
}
bool AWhyPlayerController::ServerSetClickedNumber_Validate(AColorBox* Box)
{
	return true;
}
void AWhyPlayerController::ServerSetClickedNumber_Implementation(AColorBox* Box)
{
	if (Role == ROLE_Authority)
	{
		Box->ClickedBy(GetPawn());

		
		if (Box->GetClickedNumber() == -1)
		{
			if (Box->bIsActive)
			{
				Box->SetActive(false);
			}
			else
			{
				Box->SetActive(true);
			}
		}
		Box->SetClickedNumber();
		Box->SampleFunction(GetPawn(),Box->IsActive());
		
	}
}


void AWhyPlayerController::SetDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	//Pawn의 Mesh를 변수로 받아온 위치로 이동
	AWhyPawn* Pawn = Cast<AWhyPawn>(MyPawn);
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);

	//플레이어의 위치를 변경함
	Pawn->SetMeshLocation(DestLocation);
	//이동할곳을 알려주는 메시를 플레이어의 위치로 리셋
	Pawn->SetMoveToHereMeshLocation(Pawn->GetMeshLocation());
	//현재 플레이어가 위치한 박스의 스테이지를 받아와 플레이어의 스테이지를 변경해주는 함수 호출
	
	Pawn->GetOverlappedActor();

	ServerSetStage(Pawn->GetStage());
	
	
	
}

void AWhyPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		
		AColorBox* HitActor = Cast<AColorBox>(Hit.Actor);
		APawn* const MyPawn = GetPawn();

		//Pawn의 Mesh를 변수로 받아온 위치로 이동
		AWhyPawn* Pawn = Cast<AWhyPawn>(MyPawn);
		if (HitActor)
		{
			float Distance = (HitActor->GetActorLocation() - Pawn->GetMeshLocation()).Size();

			if (StartPoint)
			{
				if (Distance < 600.0f&&Distance>200.0f)
				{
					Location = HitActor->GetSpawnPoint();
					Pawn->SetMoveToHereMeshLocation(Location);
					BecomeBox = HitActor;
					IsSetLocation = true;
				}
			}
			else
			{
				SetDestination(HitActor->GetSpawnPoint());
				StartPoint = true;
				Pawn->SetMyMaterial();
				ServerSetPlayerRandomName();

				AWhyPlayerState* WhyPlayerState = Cast<AWhyPlayerState>(PlayerState);
				GetPlayerAndSetTargetColor();
				NowBox = HitActor;
				Pawn->SetShowPlayerNameText(WhyPlayerState->GetPlayerRandomName());
			}
		}
	}
}
bool AWhyPlayerController::GetIsSetLocation()
{
	return IsSetLocation;
}
void AWhyPlayerController::SetIsSetLocation(bool State)
{
	IsSetLocation = State;
}

FVector AWhyPlayerController::GetLocation()
{
	return Location;
}


int32 AWhyPlayerController::GetPlayerNumber()
{
	return PlayerNumber;
}

void AWhyPlayerController::SetPlayerNumber(int32 Num)
{
	PlayerNumber = Num;
}


void AWhyPlayerController::GetPlayerAndSetTargetColor()
{
	AWhyPlayerState* WhyPlayerState = Cast<AWhyPlayerState>(this->PlayerState);
	AWhyPawn* Pawn = Cast<AWhyPawn>(GetPawn());
	if (WhyPlayerState != NULL &&Pawn!=NULL)
	{
		PlayerNumber = WhyPlayerState->PlayerNumber;
		TargetNumber = WhyPlayerState->TargetNumber;
		PlayerRandomName = WhyPlayerState->PlayerRandomName;
		
		Pawn->SetPlayerAndTargetNumber(PlayerNumber, TargetNumber);
		
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWhyPawn::StaticClass(), FoundActors);

		for (AActor* FoundPawn : FoundActors)
		{
			AWhyPawn* WhyPawn = Cast<AWhyPawn>(FoundPawn);
			AWhyPlayerState* State = Cast<AWhyPlayerState>(WhyPawn->PlayerState);

			if (TargetNumber == State->PlayerNumber)
			{
				WhyPawn->SetTargetPlayerMaterial();
			}
		}
	}
	

}

void AWhyPlayerController::SetVisiblePlayer()
{
	AWhyPawn* Pawn = Cast<AWhyPawn>(GetPawn());

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWhyPawn::StaticClass(), FoundActors);

	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);

	if (Pawn != NULL)
	{
			if (Pawn->GetStage())//이 플레이어의 Stage 가 true 이면
			{
				for (AActor* FoundPawn : FoundActors)
				{
					AWhyPawn* WhyPawn = Cast<AWhyPawn>(FoundPawn);
					
					if (WhyPawn->GetStage())//다른 플레이어의 Stage가 true 일때
					{
						WhyPawn->SetMeshVisible(true);
					}
					else
					{
						WhyPawn->SetMeshVisible(false);
					}
				}
			}
			else//이 플레이어의 stage가 false이면
			{
				for (AActor* FoundPawn : FoundActors)
				{
					AWhyPawn* WhyPawn = Cast<AWhyPawn>(FoundPawn);
					
					if (WhyPawn->GetStage())//다른 플레이어의 Stage가 true 일때 이 다른 플레이어는 안보임
					{
						WhyPawn->SetMeshVisible(false);
					}
					else
					{
						WhyPawn->SetMeshVisible(true);
					}
				}
			}
	}
}

bool AWhyPlayerController::ServerSetHP_Validate(int32 Num)
{
	return true;
}

void AWhyPlayerController::ServerSetHP_Implementation(int32 Num)
{
	if (Role == ROLE_Authority)
	{
		SetHP(Num);
	}
}

bool AWhyPlayerController::ServerSetMP_Validate(int32 Num)
{
	return true;
}

void AWhyPlayerController::ServerSetMP_Implementation(int32 Num)
{
	if (Role == ROLE_Authority)
	{
		SetMP(Num);
	}
}

void AWhyPlayerController::SetHP(int32 Num)
{
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);

	if (State->GetHP() - Num > 0)
	{
		State->SetHP(Num);
	}
	else
	{
		State->SetHP(State->GetHP());
		State->SetIsDie(true);
		ServerAddCatchedPlayerNumber(State->GetPlayerNumber());

		if (!State->GetIsDieTime())
		{
			State->SetIsDieTime(true);
		}
		else
		{
			State->SetHP(-100);
			State->SetMP(100 - State->GetMP());
			State->SetIsDie(false);
			State->SetIsDieTime(false);


		}
	}

	
}
void AWhyPlayerController::SetMP(int32 Num)
{
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);

	State->SetMP(Num);
	
}

void AWhyPlayerController::AttempHPMP(int32 Num)
{
	if (Num > 0)
	{
		SetHP(Num);
		if (Role < ROLE_Authority)
		{
			ServerSetHP(Num);
		}
	}
	else if(Num<0)
	{
		SetMP(Num);
		if (Role < ROLE_Authority)
		{
			ServerSetMP(Num);
		}
	}
}


AColorBox* AWhyPlayerController::GetBecomeBoxReference()
{
	return BecomeBox;
}
AColorBox* AWhyPlayerController::GetNowBoxReference()
{
	return NowBox;
}

void AWhyPlayerController::SetNowBox()
{
	NowBox = BecomeBox;
}

float AWhyPlayerController::GetHP()
{
	return HP/100.0;
}
float AWhyPlayerController::GetMP()
{
	return MP/100.0;
}
void AWhyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWhyPlayerController, HP);
	DOREPLIFETIME(AWhyPlayerController, MP);
	DOREPLIFETIME(AWhyPlayerController, OutChatMessage);
	DOREPLIFETIME(AWhyPlayerController, PlayerRandomName);
	DOREPLIFETIME(AWhyPlayerController, IsActiveDieSay);
	DOREPLIFETIME(AWhyPlayerController, SayColor);
	DOREPLIFETIME(AWhyPlayerController, TextVisible);



}

void AWhyPlayerController::SetCanSetDeathTime(bool Value)
{
	CanSetDeathTime = Value;
}

bool AWhyPlayerController::GetCanSetDeathTime()
{
	return CanSetDeathTime;
}

void AWhyPlayerController::SetOutChatMessage(FString Message)
{
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(this->PlayerState);

	OutChatMessage = FText::FromString(PS->PlayerRandomName + " : " + Message);
}

FText AWhyPlayerController::GetOutChatMessage()
{
	return OutChatMessage;
}

FString AWhyPlayerController::GetPlayerRandomName()
{
	return PlayerRandomName;
}

bool AWhyPlayerController::ServerSetIsActiveDieSay_Validate(bool Value)
{
	return true;
}
void AWhyPlayerController::ServerSetIsActiveDieSay_Implementation(bool Value)
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);

	GS->SetIsActiveDieSay(Value);
	GS->SetSayPlayerNumber(PS->GetPlayerNumber());
}

bool AWhyPlayerController::GetIsActiveDieSay()
{
	return IsActiveDieSay;
}

bool AWhyPlayerController::ServerSayColor_Validate(bool Value)
{
	return true;
}
void AWhyPlayerController::ServerSayColor_Implementation(bool Value)
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());

	GS->SetSayColor(Value);
}

bool AWhyPlayerController::GetSayColor()
{
	return SayColor;
}

bool AWhyPlayerController::ServerTextVisible_Validate(bool Value)
{
	return true;
}

void AWhyPlayerController::ServerTextVisible_Implementation(bool Value)
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->SetTextVisible(Value);
}

bool AWhyPlayerController::GetTextVisible()
{
	return TextVisible;
}

void AWhyPlayerController::SetPlayerScore(int32 Num)
{
	ServerSetPlayerScore(Num);
}
void AWhyPlayerController::ServerSetPlayerScore_Implementation(int32 Num)
{
	

	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
	PS->SetPlayerScore(Num);

	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->SetPlayersScore(PS->GetPlayerNumber(), PS->GetPlayerScore());

	
}
bool AWhyPlayerController::ServerSetPlayerScore_Validate(int32 Num)
{
	return true;
}

bool AWhyPlayerController::ServerSetSortScore_Validate()
{
	return true;
}
void AWhyPlayerController::ServerSetSortScore_Implementation()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->GetSortedScore();
}


bool AWhyPlayerController::ServerAddChangeCost_Validate()
{
	return true;
}
void AWhyPlayerController::ServerAddChangeCost_Implementation()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->AddChangeCost();
}

bool AWhyPlayerController::ServerResetChangeCost_Validate()
{
	return true;
}
void AWhyPlayerController::ServerResetChangeCost_Implementation()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->ResetChangeCost();
}

bool AWhyPlayerController::ServerSetIsDie_Validate(bool Value)
{
	return true;
}
void AWhyPlayerController::ServerSetIsDie_Implementation(bool Value)
{
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);
	State->SetIsDie(Value);
	AWhyPawn* Pawn = Cast<AWhyPawn>(GetPawn());
	if (Pawn)
	{
		if (State->GetIsDie())
		{
			Pawn->SetPlayerMeshVisible(false);
		}
		else
		{
			Pawn->SetPlayerMeshVisible(true);
		}
		
	}
}

void AWhyPlayerController::SetIsDie(bool Value)
{
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
	PS->SetIsDie(Value);
}

void AWhyPlayerController::ServerSetStage_Implementation(bool Value)
{
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
	AWhyPawn* Pawn = Cast<AWhyPawn>(GetPawn());
	PS->SetWhatStage(Pawn->GetStage());
}
bool AWhyPlayerController::ServerSetStage_Validate(bool Value)
{
	return true;
}

bool AWhyPlayerController::ServerSetGetPoint_Validate(int32 Value)
{
	return true;
}
void AWhyPlayerController::ServerSetGetPoint_Implementation(int32 Value)
{
	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
	PS->SetGetPoint(Value);

	AWhyGameState* GameState = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GameState->SetPlayersGetPoint(PS->GetPlayerNumber(),PS->GetGetPoint());
}
bool AWhyPlayerController::ServerAddCatchedPlayerNumber_Validate(int32 Num)
{
	return true;
}
void AWhyPlayerController::ServerAddCatchedPlayerNumber_Implementation(int32 Num)
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->AddCatchedPlayerNumber(Num);
}

bool AWhyPlayerController::ServerResetCatchedPlayerNumber_Validate()
{
	return true;
}
void AWhyPlayerController::ServerResetCatchedPlayerNumber_Implementation()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	GS->ResetCatchedPlayerNumber();
}

void AWhyPlayerController::TargetPlayerDie()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);

	TArray<int32> CatchedPlayer = GS->GetCatchedPlayerNumber();

	for (int32 i = 0; i < GS->GetCatchedPlayerNumber().Num(); i++)
	{
		if (State->GetTargetNumber() == CatchedPlayer[i])
		{
			State->SetGetPoint(State->GetGetPoint() + 3);
		}
	}

}

bool AWhyPlayerController::ServerSetPlayerRandomName_Validate()
{
	return true;
}

void AWhyPlayerController::ServerSetPlayerRandomName_Implementation()
{
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());
	AWhyPlayerState* State = Cast<AWhyPlayerState>(PlayerState);
	GS->SetPlayersName(State->GetPlayerRandomName(), State->GetPlayerNumber());
}











