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
	//��ü
	UPROPERTY(EditAnywhere,Category="Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	//���콺�� �ö��� �� ���� ���ϴ� �޽�
	UPROPERTY(EditAnywhere, Category = "OverMesh")
	UStaticMeshComponent* OverStaticMeshComponent;

	//�ڽ��� ����(����)�� ǥ���ϱ� ���� �ؽ�Ʈ
	UPROPERTY(EditAnywhere, Category = "Number")
	UTextRenderComponent* NumberText;

	//��Ʈ�� �������� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* Collision;

	//�÷��̾� ĳ���Ϳ� �ۿ��� �ݸ���
	UPROPERTY()
	UBoxComponent* OverlapComponent;

	//�� ���������� ����� ��
	UPROPERTY(VisibleAnywhere, Category = SecondStage)
	UMaterialInterface* SecondStageColor;
	UPROPERTY(VisibleAnywhere, Category = FirstStage)
	UMaterialInterface* FirstStageColor;

	//�ڽ��� ����
	UPROPERTY(EditAnywhere, Replicated, Category = ClickedNumber)
		int32 ClickedNumber=0;

	//���콺�� �ö��� �� Ŀ���� �ö��ִٴ� ���� ��Ÿ���� �޼ҵ�
	UFUNCTION()
	virtual void NotifyActorBeginCursorOver() override;
	UFUNCTION()
	virtual void NotifyActorEndCursorOver() override;

	//�÷��̾� ĳ���Ͱ� �̵��� ��ġ
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpawnPoint;
	//�̵��� ��ġ�� �޾ƿ�
	UFUNCTION()
	FVector GetSpawnPoint();

public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	APawn* ClickedInstigator;

	//������ ���� Ŭ��
	UFUNCTION(BlueprintAuthorityOnly)
	void ClickedBy(APawn* Pawn);
	//Ŭ���Ǿ��� �� Ŭ���̾�Ʈ �󿡼� ���� ��
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

	//���� ������������
	UFUNCTION(BlueprintPure)
	bool IsActive();
	UPROPERTY(Replicated)
	bool bIsActive;
	//�ڽ��� �������� �������ִ� �޼ҵ�
	UFUNCTION(BlueprintCallable)
	void SetActive(bool NewState);
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerSetActive(bool NewState);
	bool ServerSetActive_Validate(bool NewState);
	void ServerSetActive_Implementation(bool NewState);

	//�ڽ��� ������ ����
	UPROPERTY(Replicated)
	int32 Step = 0;

	UFUNCTION()
	int32 GetStep();

	//�ڽ��� ���������� ���� ��, �����̿� ��ȭ�� ���� �ʱ� ����
	UPROPERTY()
	bool IsChangePoint =true;
	UPROPERTY()
	bool IsChangePoint1 = true;
	//������ ���۵ǰ� ó�� ������ ��
	UPROPERTY()
	bool IsFirst = true;

	//�ڽ��� �����̿� ���� ������ �����ϰ� �������� �̸������� �����ϱ� ���� ������ ����
	UFUNCTION()
	void SampleFunction(APawn* Pawn,bool Stage);

	//�ڽ� ���� ���ڿ� ���� üũ ����Ʈ
	UPROPERTY()
	bool ClickedNumberChangePoint = true;
};
