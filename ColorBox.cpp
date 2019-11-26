// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorBox.h"


// Sets default values
AColorBox::AColorBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	//박스
	static ConstructorHelpers::FObjectFinder<UStaticMesh> STATICMESH(TEXT("StaticMesh'/Game/StarterContent/Shapes/BaseBox.BaseBox'"));
	if (STATICMESH.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(STATICMESH.Object);
	}

	OverStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OverStaticMeshComponent"));
	
	//마우스가 올라가면 색이 변하는 메시
	static ConstructorHelpers::FObjectFinder<UStaticMesh> OVERMESH(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (OVERMESH.Succeeded())
	{
		OverStaticMeshComponent->SetStaticMesh(OVERMESH.Object);
	}

	//Base material 설정
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BASE(TEXT("Material'/Game/Material/BaseMaterial.BaseMaterial'"));
	if (BASE.Succeeded())
	{
		OverStaticMeshComponent->SetMaterial(0, BASE.Object);
	}

	//2번째 스테이지일때의 색
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SECONDSTAGE(TEXT("MaterialInstanceConstant'/Game/Platformer/Materials/M_Metal_Red_Instance.M_Metal_Red_Instance'"));
	if (SECONDSTAGE.Succeeded())
	{
		SecondStageColor = SECONDSTAGE.Object;
	}

	//1번째 스테이지일때의 색
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

//마우스가 박스 위로 올라왔을때 어떤 박스 위에 있는 지 알려줌
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
		//현재 작업을 할 pawn을 세팅
		ClickedInstigator = Pawn;
		
		
	}
}

//박스의 높낮이에 따른 변수를 변경하고 변경점에 이르렀을때 변경하기 위한 변수를 세팅
void AColorBox::SampleFunction(APawn* Pawn,bool Stage)
{
	//클라이언트 상에서 클릭 이벤트 실행
	ClientOnClickedBy(Pawn,Stage);
	
	//박스의 스테이지가 false일때
	if (!bIsActive)
	{
		//이미 3번 클릭 된 상태라면
		if (Step == 3)
		{
			//한번 더 클릭되었을 경우 스테이지가 바뀌도록 하기위해서(4번 클릭되었을 경우 스테이지가 바뀌어야 하지만 4번째 눌리는 것에는 높낮이 변화를 주지 않음)
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
	//박스의 스테이지가 true(파랑)일때
	else
	{
		//이 박스가 게임 시작하고 처음 눌리는 것이라면(처음의 위치가 0이기 때문에 이 작업을 해주지 않을 경우 변경점으로 인식해서 색이 바뀌었던것같음)
		if (IsFirst)
		{
			Step++;
			IsFirst = false;
		}
		else
		{
			//현재의 높낮이에 대한 값이 0이라면 변경점에 대한 값을 변경
			if (Step == 0)
			{
				//한번 더 클릭되었을 경우 스테이지가 바뀌도록 하기위해서(4번 클릭되었을 경우 스테이지가 바뀌어야 하지만 4번째 눌리는 것에는 높낮이 변화를 주지 않음)
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

//박스가 클릭되었을 때 박스에 적힌 숫자가 이 박스의 상태에 따라 다르게 보이도록 하기위해
void AColorBox::ClientOnClickedBy_Implementation(APawn* Pawn,bool Stage)
{
	//이것을 일어나도록 하게한 플레이어를 받아옴
	ClickedInstigator = Pawn;
	
	//박스에 표시될 숫자
	int32 a;
	if (!Stage)
	{
		a = -1 * (ClickedNumber + 1);
	}
	else
	{
		a = ClickedNumber + 1;
	}

	//박스의 글자 변경
	NumberText->SetText(FText::FromString(FString::FromInt(a)));

	//스테이지에 따른 색 변경과 높낮이 변경 반영
	if (!Stage)
	{
		//박스의 색을 변경
		StaticMeshComponent->SetMaterial(0, SecondStageColor);
		
		//박스의 높낮이 변경
		FVector CollisionLocation;

		//박스의 숫자가 -1(0)일 경우 제자리에 내비둠
		if (ClickedNumber == -1)
		{
			CollisionLocation = Collision->GetComponentLocation();
		}
		//박스의 숫자가 0(1)일 경우 위치 변화를 주기위해 곱셈을 하면 0*x가 되기 때문에 때로 else if를 해주어야 함
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

