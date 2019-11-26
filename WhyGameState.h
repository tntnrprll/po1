// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WhyGameState.generated.h"

/**
 * 
 */
UCLASS()
class WHY_API AWhyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AWhyGameState();

	//들어온 플레이어 수
	UPROPERTY(Replicated)
	int32 InPlayerCount = 0;
	UFUNCTION(BlueprintCallable)
	void SetInPlayerCount(int32 Value);
	UFUNCTION(BlueprintCallable)
	int32 GetInPlayerCount();

	//잡히는 시간 설정에 대한 버튼이 눌려있는지 아닌지
	UPROPERTY(Replicated)
	bool IsActiveDieSay = true;

	//true=blue false=red
	UPROPERTY(Replicated)
	bool SayColor = true;
	UFUNCTION(BlueprintCallable)
	void SetSayColor(bool Value);
	UFUNCTION(BlueprintCallable)
	bool GetSayColor();
	
	UFUNCTION(BlueprintCallable)
	void SetIsActiveDieSay(bool Value);
	UFUNCTION(BlueprintCallable)
	bool GetIsActiveDieSay();

	UPROPERTY(Replicated)
	bool TextVisible = false;
	UFUNCTION(BlueprintCallable)
	void SetTextVisible(bool Value);
	UFUNCTION(BlueprintCallable)
	bool GetTextVisible();

	//박스의 상태를 변화시키는데 드는 비용
	UPROPERTY(Replicated)
	int32 ChangeCost = 1;
	UFUNCTION(BlueprintCallable)
	void AddChangeCost();
	UFUNCTION(BlueprintCallable)
	int32 GetChangeCost();
	UFUNCTION(BlueprintCallable)
	void ResetChangeCost();

	//선언한 플레이어 넘버
	UPROPERTY(Replicated)
	int32 SayPlayerNumber;
	UFUNCTION(BlueprintCallable)
	void SetSayPlayerNumber(int32 Num);
	UFUNCTION(BlueprintCallable)
	int32 GetSayPlayerNumber();

	//잡힌 플레이어들의 플레이어 넘버
	UPROPERTY(Replicated)
	TArray<int32> CatchedPlayerNumber;
	UFUNCTION(BlueprintCallable)
	void AddCatchedPlayerNumber(int32 Num);
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetCatchedPlayerNumber();
	UFUNCTION(BlueprintCallable)
	void ResetCatchedPlayerNumber();

	UPROPERTY(Replicated)
	TArray<int32> PlayersGetPoint = {0,1,1,1,1,1};
	UFUNCTION(BlueprintCallable)
	int32 GetPlayersGetPoint(int32 PlayerNumber,int32 Num);
	UFUNCTION(BlueprintCallable)
	void SetPlayersGetPoint(int32 PlayerNumber, int32 Num);


	UPROPERTY(Replicated)
	TArray<int32> PlayersScore = {0,0,0,0,0,0};
	UFUNCTION(BlueprintCallable)
	int32 GetPlayersScore(int32 PlayerNumber, int32 Num);
	UFUNCTION(BlueprintCallable)
	void SetPlayersScore(int32 PlayerNumber, int32 Num);

	UPROPERTY(Replicated)
	TArray<int32> PlayerScoreMappingName = { 0,1,2,3,4,5 };
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetPlayerScoreMappingName();

	UPROPERTY(Replicated)
	TArray<FString> PlayersName = { "","","","","","" };
	UFUNCTION(BlueprintCallable)
	FString GetPlayersName(int32 PlayerNumber);

	//플레이어 점수 정렬
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetSortedScore();
	UFUNCTION(BlueprintCallable)
	void SetPlayersName(FString Name,int32 PlayerNumber);

	UPROPERTY(Replicated)
	bool bInMenu;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
