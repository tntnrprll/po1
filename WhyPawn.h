// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "WhyPlayerState.h"
#include "ColorBox.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "WhyPawn.generated.h"

UCLASS()
class WHY_API AWhyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWhyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);


public:
	UPROPERTY(EditAnywhere, Category = StaticMesh)
		UStaticMeshComponent* StaticMeshComponent;
	UFUNCTION()
		FVector GetMeshLocation();

	UFUNCTION()
		void SetMeshLocation(FVector Location);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetMeshLocation(FVector Location);
	bool ServerSetMeshLocation_Validate(FVector Location);
	void ServerSetMeshLocation_Implementation(FVector Location);

	UPROPERTY(Replicated)
		FVector Location;

	UFUNCTION()
		void SetPawnLocation(int32 Rotation);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPawnLocation(int32 Rotation);
	bool ServerSetPawnLocation_Validate(int32 Rotation);
	void ServerSetPawnLocation_Implementation(int32 Rotation);

	UPROPERTY()
		UStaticMeshComponent* MoveToHere;

	UFUNCTION()
		void SetMoveToHereMeshLocation(FVector Loc);

public:
	UPROPERTY(Replicated)
		int32 PlayerNumber;
	UPROPERTY(Replicated)
		int32 TargetNumber;

	UFUNCTION()
		void SetPlayerAndTargetNumber(int32 Num1, int32 Num2);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPlayerAndTargetNumber(int32 Num1,int32 Num2);
	bool ServerSetPlayerAndTargetNumber_Validate(int32 Num1, int32 Num2);
	void ServerSetPlayerAndTargetNumber_Implementation(int32 Num1, int32 Num2);

	UFUNCTION(BlueprintCallable)
		int32 GetPlayerNumber();
	UFUNCTION(BlueprintCallable)
		void SetPlayerNumber(int32 Num);
	UFUNCTION()
		void SetTargetNumber(int32 Num);
	UFUNCTION()
		int32 GetTargetNumber();

	UPROPERTY(EditAnywhere, Category = UpCount)
		int32 UpCount = 5;
	UPROPERTY(EditAnywhere, Category = DownCount)
		int32 DownCount = 2;

	UFUNCTION(BlueprintCallable)
		int32 GetUpCount();
	UFUNCTION(BlueprintCallable)
		void SetUpCount(int32 Value);

	UFUNCTION(BlueprintCallable)
		int32 GetDownCount();
	UFUNCTION(BlueprintCallable)
		void SetDownCount(int32 Value);


	//모든 플레이어 중에 나의 타깃 번호인 플레이어를 알아내어 색을 다르게 보이게 하기
	UFUNCTION()
		void SetTargetPlayerMaterial();
	UFUNCTION()
		void GetPlayerAndSetColor();
	UPROPERTY()
		UMaterialInterface* Material;
	UPROPERTY()
		UMaterialInterface* Material1;
	UFUNCTION()
		void SetMyMaterial();

	UPROPERTY()
		UBoxComponent* Collision;
	UFUNCTION()
		void OnOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UPROPERTY(Replicated)
		bool IsWhatStage = true;
	UFUNCTION()
		void SetStage(bool Stage);
	UFUNCTION(BlueprintCallable)
		bool GetStage();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetStage(bool Stage);
	bool ServerSetStage_Validate(bool Stage);
	void ServerSetStage_Implementation(bool Stage);

	UFUNCTION()
		void SetMeshVisible(bool State);

	UFUNCTION()
		void GetOverlappedActor();

	//mesh visible
	UFUNCTION()
		void SetPlayerMeshVisible(bool Value);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetMeshVisible(bool Value);
	bool ServerSetMeshVisible_Validate(bool Value);
	void ServerSetMeshVisible_Implementation(bool Value);

	UPROPERTY(Replicated)
		bool IsMeshVisible = true;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UParticleSystemComponent* DieEffect;

	UFUNCTION(NetMultiCast, unreliable)
		void MultiCastDieEffect();
	void MultiCastDieEffect_Implementation();
	
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* SpawnEffect;

	UFUNCTION(NetMultiCast, unreliable)
		void MultiCastSpawnEffect();
	void MultiCastSpawnEffect_Implementation();
	
	void Dying();

	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ShowPlayerName;
	UFUNCTION()
		void SetShowPlayerNameText(const FString &Name);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPlayerNameText(const FString &Name);
	bool ServerSetPlayerNameText_Validate(const FString &Name);
	void ServerSetPlayerNameText_Implementation(const FString &Name);



	UPROPERTY(Replicated)
		FString PlayerNameText="";


};
