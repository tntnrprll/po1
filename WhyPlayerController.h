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

	//�� ���尡 ����Ǿ��� �� ���������� �ٸ� �÷��̾���� ���� ������ �ʰ� �ϱ� ���� �޼ҵ�
	UFUNCTION()
	void SetVisiblePlayer();

	//��Ŭ����
	UFUNCTION()
	void InputClickPressed();
	//��Ŭ����
	UFUNCTION()
	void LeftClickPressed(); 

	virtual void SetupInputComponent() override;
	
	bool IsDieTime = false;

	//���� �÷��̾� ĳ������ ��ġ
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
	
	//�ڽ��� Ŭ������ ��
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

	//�ڽ��� ĳ���Ϳ� Ÿ�� �÷��̾ ���� ���� �ٸ��� ǥ��
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
	//ü���� ���� ����� ���� �Ǵ�
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

	//���� ��ġ�� �ڽ��� ���۷���
	UPROPERTY()
	AColorBox* NowBox;
	//������ �� �ڽ��� ���� ���۷���
	UPROPERTY()
	AColorBox* BecomeBox;
	
	UFUNCTION()
	AColorBox* GetBecomeBoxReference();
	UFUNCTION()
	AColorBox* GetNowBoxReference();

	//�̵��� ������ �� �ڽ��� ���� ���۷����� ������ ���۷����� ����
	UFUNCTION()
	void SetNowBox();

	UPROPERTY()
	bool IsCheckPoint = true;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//���� ���� �ð�����
	UPROPERTY(Replicated)
	bool WhatTime;

	//deathtime���� ���� �� �ִ� ��ȸ�� �ִ���-->PlayerState��
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

	//��� �ð��� ���� Ȱ��ȭ ����
	UPROPERTY(Replicated)
	bool IsActiveDieSay = true;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetIsActiveDieSay(bool Value);
	bool ServerSetIsActiveDieSay_Validate(bool Value);
	void ServerSetIsActiveDieSay_Implementation(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsActiveDieSay();

	//��� �ð��� ������ ���������� ���� ���� ����
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

	//�÷��̾��� ������ ������
	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(int32 Num);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerSetPlayerScore(int32 Num);
	bool ServerSetPlayerScore_Validate(int32 Num);
	void ServerSetPlayerScore_Implementation(int32 Num);

	//�ڽ��� ���¸� �ٲٴ� ��뿡 ��ȭ
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerAddChangeCost();
	bool ServerAddChangeCost_Validate();
	void ServerAddChangeCost_Implementation();

	//���尡 ������ ����� �ʱ�ȭ
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

	//��ġ�� �ڽ��� ���������� ������ ������ ����
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStage(bool Stage);
	bool ServerSetStage_Validate(bool Stage);
	void ServerSetStage_Implementation(bool Stage);


	bool endvalue = false;

	//������ ���� ������ ���� ����
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetGetPoint(int32 Value);
	bool ServerSetGetPoint_Validate(int32 Value);
	void ServerSetGetPoint_Implementation(int32 Value);

	//�ٸ� �÷��̾ ������� ���� �߰�
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddCatchedPlayerNumber(int32 Num);
	bool ServerAddCatchedPlayerNumber_Validate(int32 Num);
	void ServerAddCatchedPlayerNumber_Implementation(int32 Num);

	//���尡 ���� ���� �÷��̾���� ��Ƴ�
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerResetCatchedPlayerNumber();
	bool ServerResetCatchedPlayerNumber_Validate();
	void ServerResetCatchedPlayerNumber_Implementation();

	//�ڽ��� Ÿ�� �÷��̾ ������ ��
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