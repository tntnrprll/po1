// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WhyGameState.h"
#include "Engine/World.h"
#include "WhyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API AWhyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
		int32 PlayerNumber;
	UPROPERTY(Replicated)
		int32 TargetNumber;

	UPROPERTY(Replicated)
		FString PlayerRandomName;

	UFUNCTION(BlueprintCallable)
		FString GetPlayerRandomName();
	UFUNCTION(BlueprintCallable)
		void SetPlayerRandomName(FString Name);

	UFUNCTION(BlueprintCallable)
		int32 GetPlayerNumber();
	UFUNCTION(BlueprintCallable)
		int32 GetTargetNumber();
	UFUNCTION()
		void SetPlayerNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
		void SetTargetNumber();

	UPROPERTY(Replicated)
		bool SayActive = true;
	
	UFUNCTION(BlueprintCallable)
		void SetSayActive(bool Value);
	UFUNCTION(BlueprintCallable)
		bool GetSayActive();


	UPROPERTY(Replicated)
		int32 HP = 100;
	UPROPERTY(Replicated)
		int32 MP = 100;

	UFUNCTION()
		void SetHP(int32 Num);
	UFUNCTION()
		void SetMP(int32 Num);
	UFUNCTION(BlueprintCallable)
		float GetHP();
	UFUNCTION(BlueprintCallable)
		float GetMP();
	UFUNCTION()
		int32 GetIntHP();
	UFUNCTION()
		int32 GetIntMP();

	//체력을 쓸지 기력을 쓸지 판단
	UFUNCTION()
		void AttempHPMP(int32 Num);

	UPROPERTY(Replicated)
		int32 PlayerScore = 0;

	UFUNCTION()
		void SetPlayerScore(int32 Num);
	UFUNCTION(BlueprintCallable)
		int32 GetPlayerScore();

	UPROPERTY(Replicated)
		int32 UpCount = 500;
	UFUNCTION(BlueprintCallable)
		int32 GetUpCount();
	UFUNCTION(BlueprintCallable)
		void SetUpCount(int32 Num);

	UPROPERTY(Replicated)
		bool WhatStage = true;
	UFUNCTION(BlueprintCallable)
		bool GetWhatStage();
	UFUNCTION(BlueprintCallable)
		void SetWhatStage(bool Value);

	UPROPERTY(Replicated)
		bool IsDie;
	UFUNCTION(BlueprintCallable)
		void SetIsDie(bool Value);
	UFUNCTION(BlueprintCallable)
		bool GetIsDie();

	//연속해서 살아있는 시간이 길어질수록 라운드 끝나고 얻는 점수 증가
	
	UPROPERTY(Replicated)
		int32 GetPoint = 1;
	UFUNCTION(BlueprintCallable)
		void SetGetPoint(int32 Value);
	UFUNCTION(BlueprintCallable)
		int32 GetGetPoint();

	//한턴 죽어있기 위해
	UPROPERTY(Replicated)
		bool IsDieTime = false;
	UFUNCTION(BlueprintCallable)
		void SetIsDieTime(bool Value);
	UFUNCTION(BlueprintCallable)
		bool GetIsDieTime();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};