// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WhyPlayerController.h"
#include "WhyGameState.h"
#include "WhyPawn.h"
#include "Clock.generated.h"

UCLASS()
class WHY_API AClock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClock();


	//한 라운드당 시간
	UPROPERTY(VisibleAnywhere,Replicated)
	int32 CountdownTime;
	//현재가 몇 라운드인지
	UPROPERTY(VisibleAnywhere)
	int32 RoundText;

	//시간 업데이트
	void AdvanceTimer();
	//카운트 다운이 끝났을 때
	UFUNCTION()
	void CountdownHasFinished();
	FTimerHandle CountdownTimerHandle;

	void SetCountTime(int32 CountTime);

	UFUNCTION(BlueprintCallable)
	int32 GetCountdownTime();

	UFUNCTION(BlueprintCallable)
	int32 GetRoundText();

	UPROPERTY()
	int32 InNumber = 0;
	UPROPERTY()
	int32 OutNumber = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetInNumber();
	UFUNCTION(BlueprintCallable)
	int32 GetOutNumber();

	UPROPERTY()
	bool BecuseError = false;
	//움직일 곳을 정하는 시간
	UPROPERTY()
	bool IsMoveTime = true;
	UFUNCTION(BlueprintCallable)
	bool GetIsMoveTime();

	//타일 숫자 바꾸는 시간
	UPROPERTY()
	bool IsTileChangeTime = false;
	UFUNCTION(BlueprintCallable)
	bool GetIsTileChangeTime();

	//네트워크 지연에 의한 로딩시간
	UPROPERTY()
	bool IsLoadingTime = false;
	UFUNCTION(BlueprintCallable)
	bool GetIsLoadingTime();
	UPROPERTY()
	bool IsLoadingTime2 = false;
	UFUNCTION(BlueprintCallable)
	bool GetIsLoadingTime2();
	UPROPERTY()
	bool IsLoadingTime3 = false;
	UFUNCTION(BlueprintCallable)
	bool GetIsLoadingTime3();

	//선언이 있었다면
	UPROPERTY()
	bool IsDeathTime = false;

	//타일 바꾸는 시간에 시간별 단계
	UPROPERTY()
	int32 ChangeCost = 1;
	UFUNCTION(BlueprintCallable)
	int32 GetChangeCost();

	//현재 무슨 시간
	UPROPERTY()
	bool NowTime = true;

	//라운드 종료
	UPROPERTY()
	bool EndRound = false;
	UFUNCTION(BlueprintCallable)
	bool GetEndRound();

	//선언하고 다다음 라운드에 잡히게 하기 위해서
	bool DieTime = false;
	//선언되었던 스테이지 저장하기 위해
	bool SayStageSave = true;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	int32 TotalAddPoint = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
