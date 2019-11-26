// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "ColorBox.generated.h"

UCLASS()
class WHY_API AColorBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AColorBox();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//본체
	UPROPERTY(EditAnywhere,Category="Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	//마우스가 올라갔을 때 색이 변하는 메시
	UPROPERTY(EditAnywhere, Category = "OverMesh")
	UStaticMeshComponent* OverStaticMeshComponent;

	//박스의 상태(숫자)를 표시하기 위한 텍스트
	UPROPERTY(EditAnywhere, Category = "Number")
	UTextRenderComponent* NumberText;

	//루트만 설정해준 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* Collision;

	//플레이어 캐릭터와 작용할 콜리전
	UPROPERTY()
	UBoxComponent* OverlapComponent;

	//각 스테이지에 사용할 색
	UPROPERTY(VisibleAnywhere, Category = SecondStage)
	UMaterialInterface* SecondStageColor;
	UPROPERTY(VisibleAnywhere, Category = FirstStage)
	UMaterialInterface* FirstStageColor;

	//박스의 상태
	UPROPERTY(EditAnywhere, Replicated, Category = ClickedNumber)
		int32 ClickedNumber=0;

	//마우스가 올라갔을 때 커서가 올라가있다는 것을 나타내줄 메소드
	UFUNCTION()
	virtual void NotifyActorBeginCursorOver() override;
	UFUNCTION()
	virtual void NotifyActorEndCursorOver() override;

	//플레이어 캐릭터가 이동할 위치
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnPoint;
	//이동할 위치를 받아옴
	UFUNCTION()
	FVector GetSpawnPoint();

public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	APawn* ClickedInstigator;

	//누구에 의해 클릭
	UFUNCTION(BlueprintAuthorityOnly)
	void ClickedBy(APawn* Pawn);
	//클릭되었을 때 클라이언트 상에서 해줄 일
	UFUNCTION(NetMulticast, Reliable)
		void ClientOnClickedBy(APawn* Pawn,bool Stage);
	void ClientOnClickedBy_Implementation(APawn* Pawn,bool Stage);

	UFUNCTION()
	void SetClickedNumber();
	UFUNCTION()
	int32 GetClickedNumber();
	UFUNCTION()
	void SetNumberText();

	UFUNCTION()
	void ClickedNumberReset();

	//무슨 스테이지인지
	UFUNCTION(BlueprintPure)
	bool IsActive();
	UPROPERTY(Replicated)
	bool bIsActive;
	//박스의 스테이지 변경해주는 메소드
	UFUNCTION(BlueprintCallable)
	void SetActive(bool NewState);
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerSetActive(bool NewState);
	bool ServerSetActive_Validate(bool NewState);
	void ServerSetActive_Implementation(bool NewState);

	//박스의 높낮이 상태
	UPROPERTY(Replicated)
	int32 Step = 0;

	UFUNCTION()
	int32 GetStep();

	//박스의 스테이지가 변할 때, 높낮이에 변화를 주지 않기 위해
	UPROPERTY()
	bool IsChangePoint =true;
	UPROPERTY()
	bool IsChangePoint1 = true;
	//게임이 시작되고 처음 눌렸을 때
	UPROPERTY()
	bool IsFirst = true;

	//박스의 높낮이에 따른 변수를 변경하고 변경점에 이르렀을때 변경하기 위한 변수를 세팅
	UFUNCTION()
	void SampleFunction(APawn* Pawn,bool Stage);

	//박스 상태 숫자에 대한 체크 포인트
	UPROPERTY()
	bool ClickedNumberChangePoint = true;
};
