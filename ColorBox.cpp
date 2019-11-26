// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorBox.h"


// Sets default values
AColorBox::AColorBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	//�ڽ�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> STATICMESH(TEXT("StaticMesh'/Game/StarterContent/Shapes/BaseBox.BaseBox'"));
	if (STATICMESH.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(STATICMESH.Object);
	}

	OverStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OverStaticMeshComponent"));
	
	//���콺�� �ö󰡸� ���� ���ϴ� �޽�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OVERMESH(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (OVERMESH.Succeeded())
	{
		OverStaticMeshComponent->SetStaticMesh(OVERMESH.Object);
	}

	//Base material ����
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BASE(TEXT("Material'/Game/Material/BaseMaterial.BaseMaterial'"));
	if (BASE.Succeeded())
	{
		OverStaticMeshComponent->SetMaterial(0, BASE.Object);
	}

	//2��° ���������϶��� ��
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SECONDSTAGE(TEXT("MaterialInstanceConstant'/Game/Platformer/Materials/M_Metal_Red_Instance.M_Metal_Red_Instance'"));
	if (SECONDSTAGE.Succeeded())
	{
		SecondStageColor = SECONDSTAGE.Object;
	}

	//1��° ���������϶��� ��
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FIRSTSTAGE(TEXT("MaterialInstanceConstant'/Game/Platformer/Materials/M_Metal_Grey_Instance.M_Metal_Grey_Instance'"));
	if (FIRSTSTAGE.Succeeded())
	{
		FirstStageColor =FIRSTSTAGE.Object;
	}



	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetWorldScale3D(FVector(1.f,1.f,1.f));
	RootComponent = Collision;
	
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetWorldScale3D(FVector(2.5f, 2.5f, 2.5f));
	StaticMeshComponent->SetMaterial(0, FirstStageColor);

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetupAttachment(RootComponent);
	OverlapComponent->SetWorldScale3D(FVector(5.0f, 5.0f, 20.0f));
	OverlapComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	OverStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	OverStaticMeshComponent->SetupAttachment(RootComponent);
	OverStaticMeshComponent->SetWorldScale3D(FVector(3.3f,3.3f, 1.0f));
	OverStaticMeshComponent->SetVisibility(false);

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);
	SpawnPoint->SetRelativeLocation(FVector(0.0f,0.0f, 110.f));

	NumberText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NumberText"));
	NumberText->SetupAttachment(RootComponent);
	NumberText->SetRelativeLocation(FVector(-20.0f, 90.0f, 70.0f));
	NumberText->SetText(FString::FromInt(ClickedNumber));
	NumberText->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	NumberText->SetWorldSize(80.0f);

	if (Role == ROLE_Authority)
	{
		ClickedNumber = 0;
		bIsActive = true;
	}
	
	bReplicates = true;
	
}



bool AColorBox::IsActive()
{
	return bIsActive;
}

void AColorBox::SetActive(bool NewState)
{
	bIsActive = NewState;

	if (Role < ROLE_Authority)
	{
		ServerSetActive(NewState);
	}
}
bool AColorBox::ServerSetActive_Validate(bool NewState)
{
	return true;
}
void AColorBox::ServerSetActive_Implementation(bool NewState)
{
	if (Role == ROLE_Authority)
	{
		SetActive(NewState);
	}
}


void AColorBox::SetClickedNumber()
{
	if (Role == ROLE_Authority)
	{
		if (ClickedNumber == 2)
		{
			ClickedNumberReset();
		}
		else
		{
			ClickedNumber += 1;
		}
	}
}

void AColorBox::ClickedNumberReset()
{
	if (Role == ROLE_Authority)
	{
		ClickedNumber = -1;
	}
}

int32 AColorBox::GetClickedNumber()
{
	return ClickedNumber;
}

//���콺�� �ڽ� ���� �ö������ � �ڽ� ���� �ִ� �� �˷���
void AColorBox::NotifyActorBeginCursorOver()
{
	OverStaticMeshComponent->SetVisibility(true);
}

void AColorBox::NotifyActorEndCursorOver()
{
	OverStaticMeshComponent->SetVisibility(false);
}


void AColorBox::ClickedBy(APawn* Pawn)
{
	if (Role == ROLE_Authority)
	{
		//���� �۾��� �� pawn�� ����
		ClickedInstigator = Pawn;
		
		
	}
}

//�ڽ��� �����̿� ���� ������ �����ϰ� �������� �̸������� �����ϱ� ���� ������ ����
void AColorBox::SampleFunction(APawn* Pawn,bool Stage)
{
	//Ŭ���̾�Ʈ �󿡼� Ŭ�� �̺�Ʈ ����
	ClientOnClickedBy(Pawn,Stage);
	
	//�ڽ��� ���������� false�϶�
	if (!bIsActive)
	{
		//�̹� 3�� Ŭ�� �� ���¶��
		if (Step == 3)
		{
			//�ѹ� �� Ŭ���Ǿ��� ��� ���������� �ٲ�� �ϱ����ؼ�(4�� Ŭ���Ǿ��� ��� ���������� �ٲ��� ������ 4��° ������ �Ϳ��� ������ ��ȭ�� ���� ����)
			if (!IsChangePoint)
			{
				Step--;
				IsChangePoint = true;
			}
			else
			{
				IsChangePoint = false;
			}
		}
		else
		{
			Step--;
		}
	}
	//�ڽ��� ���������� true(�Ķ�)�϶�
	else
	{
		//�� �ڽ��� ���� �����ϰ� ó�� ������ ���̶��(ó���� ��ġ�� 0�̱� ������ �� �۾��� ������ ���� ��� ���������� �ν��ؼ� ���� �ٲ�����Ͱ���)
		if (IsFirst)
		{
			Step++;
			IsFirst = false;
		}
		else
		{
			//������ �����̿� ���� ���� 0�̶�� �������� ���� ���� ����
			if (Step == 0)
			{
				//�ѹ� �� Ŭ���Ǿ��� ��� ���������� �ٲ�� �ϱ����ؼ�(4�� Ŭ���Ǿ��� ��� ���������� �ٲ��� ������ 4��° ������ �Ϳ��� ������ ��ȭ�� ���� ����)
				if (!IsChangePoint1)
				{
					Step++;
					IsChangePoint1 = true;
				}
				else
				{
					IsChangePoint1 = false;
				}
			}
			else
			{
				
				Step++;
			}
		}


		
	}
}

//�ڽ��� Ŭ���Ǿ��� �� �ڽ��� ���� ���ڰ� �� �ڽ��� ���¿� ���� �ٸ��� ���̵��� �ϱ�����
void AColorBox::ClientOnClickedBy_Implementation(APawn* Pawn,bool Stage)
{
	//�̰��� �Ͼ���� �ϰ��� �÷��̾ �޾ƿ�
	ClickedInstigator = Pawn;
	
	//�ڽ��� ǥ�õ� ����
	int32 a;
	if (!Stage)
	{
		a = -1 * (ClickedNumber + 1);
	}
	else
	{
		a = ClickedNumber + 1;
	}

	//�ڽ��� ���� ����
	NumberText->SetText(FText::FromString(FString::FromInt(a)));

	//���������� ���� �� ����� ������ ���� �ݿ�
	if (!Stage)
	{
		//�ڽ��� ���� ����
		StaticMeshComponent->SetMaterial(0, SecondStageColor);
		
		//�ڽ��� ������ ����
		FVector CollisionLocation;

		//�ڽ��� ���ڰ� -1(0)�� ��� ���ڸ��� �����
		if (ClickedNumber == -1)
		{
			CollisionLocation = Collision->GetComponentLocation();
		}
		//�ڽ��� ���ڰ� 0(1)�� ��� ��ġ ��ȭ�� �ֱ����� ������ �ϸ� 0*x�� �Ǳ� ������ ���� else if�� ���־�� ��
		else if (ClickedNumber == 0)
		{
			CollisionLocation = Collision->GetComponentLocation() + FVector(0.0f, 0.0f, (ClickedNumber+1)*40.0f);
		}
		else
		{
			CollisionLocation = Collision->GetComponentLocation() + FVector(0.0f, 0.0f, ClickedNumber*40.0f);
		}
		
		Collision->SetRelativeLocation(CollisionLocation);
		
	}
	else
	{
		StaticMeshComponent->SetMaterial(0, FirstStageColor);
		
		FVector CollisionLocation;
		if (ClickedNumber == -1)
		{
			CollisionLocation = Collision->GetComponentLocation();
		}
		else if (ClickedNumber == 0)
		{
			CollisionLocation = Collision->GetComponentLocation() + FVector(0.0f, 0.0f, (ClickedNumber + 1)*-40.0f);
		}
		else
		{
			CollisionLocation = Collision->GetComponentLocation() + FVector(0.0f, 0.0f, ClickedNumber*-40.0f);
		}
		Collision->SetRelativeLocation(CollisionLocation);
	}
}

void AColorBox::SetNumberText()
{
	int32 a;
	if (!bIsActive)
	{
		a = -1 * (ClickedNumber + 1);
	}
	else
	{
		a = ClickedNumber + 1;
	}
	NumberText->SetText(FText::FromString(FString::FromInt(a)));
	
}

FVector AColorBox::GetSpawnPoint()
{
	return SpawnPoint->GetComponentLocation();
}



void AColorBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AColorBox, ClickedNumber);
	DOREPLIFETIME(AColorBox, bIsActive);
	DOREPLIFETIME(AColorBox, Step);


}

int32 AColorBox::GetStep()
{
	return Step;
}

