// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WhyPawn.h"
#include "ColorBox.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WhyPlayerState.h"
#include "Engine/World.h"
#include "WhyGameState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
#include "WhyGameInstance.h"
#include "WhyGameState.h"
#include "WhyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API AWhyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWhyPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//각 라운드가 종료되었을 때 스테이지가 다른 플레이어들이 서로 보이지 않게 하기 위한 메소드
	UFUNCTION()
	void SetVisiblePlayer();

	//우클릭시
	UFUNCTION()
	void InputClickPressed();
	//좌클릭시
	UFUNCTION()
	void LeftClickPressed(); 

	virtual void SetupInputComponent() override;
	
	bool IsDieTime = false;

	//현재 플레이어 캐릭터의 위치
	UPROPERTY()
	FVector Location;
	UFUNCTION()
	FVector GetLocation();
	UPROPERTY()
	bool IsSetLocation = false;
	UFUNCTION()
	void SetIsSetLocation(bool State);
	UFUNCTION()
	bool GetIsSetLocation();
	UFUNCTION()
	void SetDestination(const FVector DestLocation);
	UFUNCTION()
	void MoveToMouseCursor();
	
	//박스를 클릭했을 때
	UFUNCTION()
	void Clicked(AColorBox* Box);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetClickedNumber(AColorBox* Box);
	bool ServerSetClickedNumber_Validate(AColorBox* Box);
	void ServerSetClickedNumber_Implementation(AColorBox* Box);


	UPROPERTY(EditAnywhere, Category = PlayerNumber)
	int32 PlayerNumber = 0;
	UPROPERTY(EditAnywhere, Category = TargetNumber)
	int32 TargetNumber=0;

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerNumber();
	UFUNCTION(BlueprintCallable)
	void SetPlayerNumber(int32 Num);

	UPROPERTY()
	bool StartPoint = false;

	UPROPERTY()
	AWhyPlayerState* State;

	//자신의 캐릭터와 타깃 플레이어에 대해 색을 다르게 표시
	UFUNCTION()
	void GetPlayerAndSetTargetColor();

	UPROPERTY(Replicated)
	int32 HP=100;
	UPROPERTY(Replicated)
	int32 MP=100;

	UFUNCTION()
	void SetHP(int32 Num);
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerSetHP(int32 Num);
	bool ServerSetHP_Validate(int32 Num);
	void ServerSetHP_Implementation(int32 Num);
	//체력을 쓸지 기력을 쓸지 판단
	UFUNCTION()
	void AttempHPMP(int32 Num);

	UFUNCTION()
	void SetMP(int32 Num);
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerSetMP(int32 Num);
	bool ServerSetMP_Validate(int32 Num);
	void ServerSetMP_Implementation(int32 Num);

	UFUNCTION(BlueprintCallable)
	float GetHP();
	UFUNCTION(BlueprintCallable)
	float GetMP();

	UPROPERTY()
	int32 NowBoxNumber;
	UPROPERTY()
	int32 BecomeBoxNumber;

	//현재 위치한 박스의 레퍼런스
	UPROPERTY()
	AColorBox* NowBox;
	//앞으로 갈 박스에 대한 레퍼런스
	UPROPERTY()
	AColorBox* BecomeBox;
	
	UFUNCTION()
	AColorBox* GetBecomeBoxReference();
	UFUNCTION()
	AColorBox* GetNowBoxReference();

	//이동후 앞으로 갈 박스에 대한 레퍼런스를 현재의 레퍼런스로 변경
	UFUNCTION()
	void SetNowBox();

	UPROPERTY()
	bool IsCheckPoint = true;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//현재 무슨 시간인지
	UPROPERTY(Replicated)
	bool WhatTime;

	//deathtime으로 만들 수 있는 기회가 있는지-->PlayerState로
	UPROPERTY()
	bool CanSetDeathTime = true;
	UFUNCTION()
	void SetCanSetDeathTime(bool Value);
	UFUNCTION()
	bool GetCanSetDeathTime();


	UPROPERTY(Replicated)
	FText OutChatMessage;

	UFUNCTION(BlueprintCallable)
	void SetOutChatMessage(FString Message);

	UFUNCTION(BlueprintCallable)
	FText GetOutChatMessage();

	UPROPERTY(Replicated)
	FString PlayerRandomName;

	UFUNCTION(BlueprintCallable)
	FString GetPlayerRandomName();

	//잡는 시간에 대한 활성화 여부
	UPROPERTY(Replicated)
	bool IsActiveDieSay = true;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetIsActiveDieSay(bool Value);
	bool ServerSetIsActiveDieSay_Validate(bool Value);
	void ServerSetIsActiveDieSay_Implementation(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsActiveDieSay();

	//잡는 시간에 잡히는 스테이지에 대한 정보 저장
	UPROPERTY(Replicated)
	bool SayColor = true;
	UFUNCTION(BlueprintCallable)
	bool GetSayColor();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSayColor(bool Value);
	bool ServerSayColor_Validate(bool Value);
	void ServerSayColor_Implementation(bool Value);

	

	UPROPERTY(Replicated)
	bool TextVisible = false;
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerTextVisible(bool Value);
	bool ServerTextVisible_Validate(bool Value);
	void ServerTextVisible_Implementation(bool Value);
	UFUNCTION(BlueprintCallable)
	bool GetTextVisible();

	//플레이어의 점수를 설정함
	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(int32 Num);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetPlayerScore(int32 Num);
	bool ServerSetPlayerScore_Validate(int32 Num);
	void ServerSetPlayerScore_Implementation(int32 Num);

	//박스의 상태를 바꾸는 비용에 변화
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerAddChangeCost();
	bool ServerAddChangeCost_Validate();
	void ServerAddChangeCost_Implementation();

	//라운드가 끝나고 비용을 초기화
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerResetChangeCost();
	bool ServerResetChangeCost_Validate();
	void ServerResetChangeCost_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetIsDie(bool Value);
	bool ServerSetIsDie_Validate(bool Value);
	void ServerSetIsDie_Implementation(bool Value);

	UFUNCTION()
	void SetIsDie(bool Value);

	//위치한 박스의 스테이지의 정보를 가져와 설정
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStage(bool Stage);
	bool ServerSetStage_Validate(bool Stage);
	void ServerSetStage_Implementation(bool Stage);


	bool endvalue = false;

	//앞으로 얻을 점수에 대한 설정
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetGetPoint(int32 Value);
	bool ServerSetGetPoint_Validate(int32 Value);
	void ServerSetGetPoint_Implementation(int32 Value);

	//다른 플레이어를 잡았을때 점수 추가
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddCatchedPlayerNumber(int32 Num);
	bool ServerAddCatchedPlayerNumber_Validate(int32 Num);
	void ServerAddCatchedPlayerNumber_Implementation(int32 Num);

	//라운드가 지나 잡힌 플레이어들이 살아남
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerResetCatchedPlayerNumber();
	bool ServerResetCatchedPlayerNumber_Validate();
	void ServerResetCatchedPlayerNumber_Implementation();

	//자신의 타깃 플레이어가 잡혔을 때
	void TargetPlayerDie();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetSortScore();
	bool ServerSetSortScore_Validate();
	void ServerSetSortScore_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPlayerRandomName();
	bool ServerSetPlayerRandomName_Validate();
	void ServerSetPlayerRandomName_Implementation();

};