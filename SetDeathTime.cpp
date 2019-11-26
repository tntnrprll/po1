// Fill out your copyright notice in the Description page of Project Settings.

#include "SetDeathTime.h"

bool USetDeathTime::GetIsColor()
{
	return IsColor;
}

void USetDeathTime::SetIsColor()
{
	IsColor = !IsColor;
}

void USetDeathTime::SetIsDeathTime(bool Value)
{
	IsDeathTime = Value;
}

bool USetDeathTime::GetIsDeathTime()
{
	return IsDeathTime;
}
