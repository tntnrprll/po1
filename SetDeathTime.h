// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SetDeathTime.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API USetDeathTime : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//»¡°­ÀÌ³Ä ÆÄ¶ûÀÌ³Ä
	UPROPERTY()
		bool IsColor = true;//»¡°­
	
	UFUNCTION(BlueprintCallable)
		void SetIsColor();
	UFUNCTION(BlueprintCallable)
		bool GetIsColor();

	UPROPERTY()
		bool IsDeathTime = false;

	UFUNCTION()
		void SetIsDeathTime(bool Value);
	UFUNCTION()
		bool GetIsDeathTime();

	
};
