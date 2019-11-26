// Fill out your copyright notice in the Description page of Project Settings.

#include "WhyPlayerState.h"
#include "UnrealNetwork.h"

void AWhyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWhyPlayerState, PlayerNumber);
	DOREPLIFETIME(AWhyPlayerState, TargetNumber);
	DOREPLIFETIME(AWhyPlayerState, PlayerRandomName);
	DOREPLIFETIME(AWhyPlayerState, SayActive);
	DOREPLIFETIME(AWhyPlayerState, PlayerScore);
	DOREPLIFETIME(AWhyPlayerState, HP);
	DOREPLIFETIME(AWhyPlayerState, MP);
	DOREPLIFETIME(AWhyPlayerState, UpCount);
	DOREPLIFETIME(AWhyPlayerState, WhatStage);
	DOREPLIFETIME(AWhyPlayerState, IsDie);
	DOREPLIFETIME(AWhyPlayerState, GetPoint);
	DOREPLIFETIME(AWhyPlayerState, IsDieTime);



}

int32 AWhyPlayerState::GetPlayerNumber()
{
	return PlayerNumber;
}

int32 AWhyPlayerState::GetTargetNumber()
{
	return TargetNumber;
}

void AWhyPlayerState::SetTargetNumber()
{

	if (PlayerNumber == 5)
	{
		TargetNumber = 1;
	}
	else
	{
		TargetNumber = PlayerNumber + 1;
	}

	
}

void AWhyPlayerState::SetPlayerNumber(int32 Number)
{

	PlayerNumber = Number;

}

FString AWhyPlayerState::GetPlayerRandomName()
{
	return PlayerRandomName;
}
void AWhyPlayerState::SetPlayerRandomName(FString Name)
{
	PlayerRandomName = Name;
}

void AWhyPlayerState::SetSayActive(bool Value)
{
	SayActive = Value;
}

bool AWhyPlayerState::GetSayActive()
{
	return SayActive;
}


void AWhyPlayerState::SetHP(int32 Num)
{
	HP = HP - Num;
	
	
}
void AWhyPlayerState::SetMP(int32 Num)
{
	if (MP + Num >= 0)
	{
		MP = MP + Num;
		//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("%f"), MP));
	}

}


void AWhyPlayerState::AttempHPMP(int32 Num)
{
	if (Num > 0)
	{
		SetHP(Num);
	}
	else if (Num < 0)
	{
		SetMP(Num);
	}
}

int32 AWhyPlayerState::GetIntHP()
{
	return HP;
}
int32 AWhyPlayerState::GetIntMP()
{
	return MP;
}

float AWhyPlayerState::GetHP()
{
	//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("%f"),HP));
	return HP / 100.0;
}
float AWhyPlayerState::GetMP()
{
	//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("%f"), MP));
	return MP / 100.0;
}

void AWhyPlayerState::SetPlayerScore(int32 Num)
{
	PlayerScore += Num;
	//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("playernumber%d playerscore%d"), GetPlayerNumber(), GetPlayerScore()));
}
int32 AWhyPlayerState::GetPlayerScore()
{
	return PlayerScore;
}

void AWhyPlayerState::SetUpCount(int32 Num)
{
	UpCount = Num;
}

int32 AWhyPlayerState::GetUpCount()
{
	return UpCount;
}

void AWhyPlayerState::SetWhatStage(bool Value)
{
	WhatStage = Value;
}

bool AWhyPlayerState::GetWhatStage()
{
	return WhatStage;
}

void AWhyPlayerState::SetIsDie(bool Value)
{
	IsDie = Value;
}
bool AWhyPlayerState::GetIsDie()
{
	return IsDie;
}

void AWhyPlayerState::SetGetPoint(int32 Value)
{
	GetPoint = Value;
}
int32 AWhyPlayerState::GetGetPoint()
{
	return GetPoint;
}
void AWhyPlayerState::SetIsDieTime(bool Value)
{
	IsDieTime = Value;
}
bool AWhyPlayerState::GetIsDieTime()
{
	return IsDieTime;
}

