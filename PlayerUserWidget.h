// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API UPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		TArray<FText> Chatting;

	UPROPERTY()
		int32 MaxChattingNumber = 30;

	UPROPERTY()
	FText Message;

	UFUNCTION(BlueprintCallable)
		void UpdateMessage(FText MessageText);
	
	UFUNCTION(BlueprintCallable)
		TArray<FText> GetChatting();
	
};
