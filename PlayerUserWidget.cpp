// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUserWidget.h"

void UPlayerUserWidget::UpdateMessage(FText MessageText)
{
	if (Chatting.Num() > MaxChattingNumber)
	{
		Chatting.RemoveAt(0);
	}
	else
	{
		Chatting.Add(MessageText);
	}
	
}

TArray<FText> UPlayerUserWidget::GetChatting()
{
	return Chatting;
}


