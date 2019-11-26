// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WhyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API UWhyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWhyGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerInputName;

	UFUNCTION(BlueprintCallable)
		void SetPlayerInputName(FString Name);
	UFUNCTION(BlueprintCallable)
		FString GetPlayerInputName();
	
	
};
