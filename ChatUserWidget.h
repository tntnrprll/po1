// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API UChatUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Sender;
	UPROPERTY(EditAnywhere)
	FText Message;
	

};
