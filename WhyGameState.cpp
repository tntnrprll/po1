// Fill out your copyright notice in the Description page of Project Settings.

#include "WhyGameState.h"
#include "UnrealNetwork.h"


AWhyGameState::AWhyGameState()
{
	bInMenu = false;
}

void AWhyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWhyGameState, IsActiveDieSay);
	DOREPLIFETIME(AWhyGameState, SayColor);
	DOREPLIFETIME(AWhyGameState, TextVisible);
	DOREPLIFETIME(AWhyGameState, ChangeCost);
	DOREPLIFETIME(AWhyGameState, SayPlayerNumber);
	DOREPLIFETIME(AWhyGameState, CatchedPlayerNumber);
	DOREPLIFETIME(AWhyGameState, PlayersScore);
	DOREPLIFETIME(AWhyGameState, PlayersGetPoint);
	DOREPLIFETIME(AWhyGameState, PlayerScoreMappingName);
	DOREPLIFETIME(AWhyGameState, PlayersName);
	DOREPLIFETIME(AWhyGameState, bInMenu);
	DOREPLIFETIME(AWhyGameState, InPlayerCount);


}

void AWhyGameState::SetInPlayerCount(int32 Value)
{
	InPlayerCount = Value;
}
int32 AWhyGameState::GetInPlayerCount()
{
	return InPlayerCount;
}

bool AWhyGameState::GetIsActiveDieSay()
{
	return IsActiveDieSay;
}

void AWhyGameState::SetIsActiveDieSay(bool Value)
{
	IsActiveDieSay = Value;
}

bool AWhyGameState::GetSayColor()
{
	return SayColor;
}
void AWhyGameState::SetSayColor(bool Value)
{
	SayColor = Value;
}
bool AWhyGameState::GetTextVisible()
{
	return TextVisible;
}
void AWhyGameState::SetTextVisible(bool Value)
{
	TextVisible = Value;
}
void AWhyGameState::AddChangeCost()
{
	ChangeCost++;
}
int32 AWhyGameState::GetChangeCost()
{
	return ChangeCost;
}
void AWhyGameState::ResetChangeCost()
{
	ChangeCost = 1;
}
void AWhyGameState::SetSayPlayerNumber(int32 Num)
{
	SayPlayerNumber = Num;
}
int32 AWhyGameState::GetSayPlayerNumber()
{
	return SayPlayerNumber;
}

void AWhyGameState::AddCatchedPlayerNumber(int32 Num)
{
	CatchedPlayerNumber.Add(Num);
}
TArray<int32> AWhyGameState::GetCatchedPlayerNumber()
{
	return CatchedPlayerNumber;
}
void AWhyGameState::ResetCatchedPlayerNumber()
{
	CatchedPlayerNumber.Empty();
}


int32 AWhyGameState::GetPlayersGetPoint(int32 PlayerNumber, int32 Num)
{
	return PlayersGetPoint[PlayerNumber];
}
void AWhyGameState::SetPlayersGetPoint(int32 PlayerNumber, int32 Num)
{
	PlayersGetPoint[PlayerNumber] = Num;
}
int32 AWhyGameState::GetPlayersScore(int32 PlayerNumber, int32 Num)
{
	return PlayersScore[PlayerNumber];
}
void AWhyGameState::SetPlayersScore(int32 PlayerNumber, int32 Num)
{
	PlayersScore[PlayerNumber] = Num;
}

//플레이어 점수 정렬
TArray<int32> AWhyGameState::GetSortedScore()
{
	
	int32 NameTemp;
	int32 temp;
	for (int32 i = 1; i <= 5-1; i++)
	{
		for (int32 j = 1; j <= 5-i-1; j++)
		{
			if (PlayersScore[j] > PlayersScore[j+1])
			{
				temp = PlayersScore[j+1];
				NameTemp = PlayerScoreMappingName[j + 1];
				PlayersScore[j+1] = PlayersScore[j];
				PlayerScoreMappingName[j + 1] = PlayerScoreMappingName[j];
				PlayersScore[j] = temp;
				PlayerScoreMappingName[j] = NameTemp;
			}
		}
	}

	return PlayersScore;
}

TArray<int32> AWhyGameState::GetPlayerScoreMappingName()
{
	return PlayerScoreMappingName;
}


FString AWhyGameState::GetPlayersName(int32 PlayerNumber)
{
	
	return PlayersName[PlayerNumber];
}

void AWhyGameState::SetPlayersName(FString Name,int32 PlayerNumber)
{
	PlayersName[PlayerNumber] = Name;
}


















