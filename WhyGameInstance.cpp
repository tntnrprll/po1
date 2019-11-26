// Fill out your copyright notice in the Description page of Project Settings.

#include "WhyGameInstance.h"

UWhyGameInstance::UWhyGameInstance()
{

}

void UWhyGameInstance::SetPlayerInputName(FString Name)
{




	PlayerInputName = Name;
}

FString UWhyGameInstance::GetPlayerInputName()

{
	return PlayerInputName;
}


