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


	//�� ����� �ð�
	UPROPERTY(VisibleAnywhere,Replicated)
	int32 CountdownTime;
	//���簡 �� ��������
	UPROPERTY(VisibleAnywhere)
	int32 RoundText;

	//�ð� ������Ʈ
	void AdvanceTimer();
	//ī��Ʈ �ٿ��� ������ ��
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
	//������ ���� ���ϴ� �ð�
	UPROPERTY()
	bool IsMoveTime = true;
	UFUNCTION(BlueprintCallable)
	bool GetIsMoveTime();

	//Ÿ�� ���� �ٲٴ� �ð�
	UPROPERTY()
	bool IsTileChangeTime = false;
	UFUNCTION(BlueprintCallable)
	bool GetIsTileChangeTime();

	//��Ʈ��ũ ������ ���� �ε��ð�
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

	//������ �־��ٸ�
	UPROPERTY()
	bool IsDeathTime = false;

	//Ÿ�� �ٲٴ� �ð��� �ð��� �ܰ�
	UPROPERTY()
	int32 ChangeCost = 1;
	UFUNCTION(BlueprintCallable)
	int32 GetChangeCost();

	//���� ���� �ð�
	UPROPERTY()
	bool NowTime = true;

	//���� ����
	UPROPERTY()
	bool EndRound = false;
	UFUNCTION(BlueprintCallable)
	bool GetEndRound();

	//�����ϰ� �ٴ��� ���忡 ������ �ϱ� ���ؼ�
	bool DieTime = false;
	//����Ǿ��� �������� �����ϱ� ����
	bool SayStageSave = true;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	int32 TotalAddPoint = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
