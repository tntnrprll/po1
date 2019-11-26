// Fill out your copyright notice in the Description page of Project Settings.

#include "Clock.h"
#include "Engine/World.h"


// Sets default values
AClock::AClock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (Role == ROLE_Authority)
	{
		CountdownTime = 60;
	}
	RoundText = 1;
	bReplicates = true;

}


// Called when the game starts or when spawned
void AClock::BeginPlay()
{
	Super::BeginPlay();

	//Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AClock::AdvanceTimer, 1.0f, true);

}

void AClock::AdvanceTimer()
{
	
	AWhyGameState* WhyGameState = Cast<AWhyGameState>(GetWorld()->GetGameState());

	if (WhyGameState->GetInPlayerCount() == 5)
	{
		--CountdownTime;
		if (CountdownTime < 1)
		{
			CountdownHasFinished();
		}
	}

}

void AClock::CountdownHasFinished()
{
	//���������� �ִ� �÷��̾� ��
	InNumber = 0;
	OutNumber = 0;

	AWhyPlayerController * MyPc = Cast<AWhyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AWhyPawn * MyPawn = Cast<AWhyPawn>(MyPc->GetPawn());
	AWhyPlayerState* MyPS = Cast<AWhyPlayerState>(MyPc->PlayerState);
	AWhyGameState* GS = Cast<AWhyGameState>(GetWorld()->GetGameState());

	if (Role < ROLE_Authority)
	{
		if (MyPc&&MyPawn&&MyPS)
		{
			//�����̴� �ð��� ��
			if (IsMoveTime)
			{
				NowTime = true;
				MyPc->WhatTime = NowTime;
				SetCountTime(60);
				IsTileChangeTime = true;
				IsMoveTime = false;
			}
			//�������� �ٲٴ� �ð��� ��
			else if (IsTileChangeTime)
			{
				NowTime = false;
				MyPc->WhatTime = NowTime;
				IsTileChangeTime = false;
				IsLoadingTime = true;

				//��ġ�� ����Ǿ��ٸ�
				if (MyPc->GetIsSetLocation())
				{
					//���� �ڽ��� �� �ڽ��� ���۷������� Step�� �޾ƿ� ���� ���� �ִ��� ������ �Ǵ��Ͽ� �̵�
					int32 StepSub = MyPc->GetBecomeBoxReference()->GetStep() - MyPc->GetNowBoxReference()->GetStep();

					if (StepSub < 0)
					{
						MyPS->SetMP(StepSub * 10);
					}
					else
					{
						MyPS->SetHP(StepSub * 10);
						//HP�� 0������ ��� ���������� ����
						if (MyPS->GetHP() <= 0)
						{
							MyPS->SetIsDieTime(false);
						}
					}
					//�÷��̾�޽��� ��ġ�� �ٲ�
					MyPc->SetDestination(MyPc->GetLocation());
					//�÷��̾��� ��ġ ������ �Ϸ�Ǿ� ��ġ�� ������ �ʾҴٴ� ���·� ����
					MyPc->SetIsSetLocation(false);
					//���� �ִ� ��ġ�� �ڽ��� ����
					MyPc->SetNowBox();
				}
				else
				{
					MyPc->SetDestination(MyPawn->GetMeshLocation());
				}
				


				SetCountTime(1);



			}
			//�ε��ð�
			else if (IsLoadingTime)
			{
				IsLoadingTime = false;
				IsLoadingTime2 = true;
				//IsMoveTime = true;



				if (!GS->GetIsActiveDieSay())//������ �Ǿ��ִ� ���¶��
				{
					if (!DieTime)//����ǰ� �� ���尡 ������ �ʾҴٸ�
					{  }
					else//����ǰ� �� ���尡 �����ٸ�
					{
						if (MyPS->GetWhatStage() == GS->GetSayColor())//����� ���������� ���� ���������� ������ Ȯ��
						{
							MyPS->SetHP(100);//�׾��ٴ� �ǹ̿��� ü���� 0���� �������
							MyPS->SetIsDieTime(false);
							MyPS->SetGetPoint(MyPS->GetGetPoint()/2);
							MyPS->SetIsDie(true);
							//MyPc->ServerSetIsActiveDieSay(true);
							MyPawn->SetPlayerMeshVisible(false);//�����ٸ� �÷��̾ ������ �ʵ���

							MyPc->ServerAddCatchedPlayerNumber(MyPS->GetPlayerNumber());//���� �÷��̾ �����ϴ� ������ �� ���� �÷��̾� �߰�

						}
						
					}

				}
				//�÷��̾���� ���������� ���� ���̰� �Ⱥ��̰� ����
				MyPc->SetVisiblePlayer();

				SetCountTime(1);
				
			}
			else if (IsLoadingTime2)
			{
				IsLoadingTime2 = false;
				IsLoadingTime3 = true;
				//IsMoveTime = true;

				TotalAddPoint = 0;
				if (!GS->GetIsActiveDieSay())//������ �Ǿ��ִ� ���¶��
				{
					if (!DieTime)//����ǰ� �� ���尡 ������ �ʾҴٸ�
					{
						DieTime = true;
						
						
					}
					else//����ǰ� �� ���尡 �����ٸ�
					{
						if (MyPS->GetPlayerNumber() == GS->GetSayPlayerNumber())//���� ��ȣ�� ������ �÷��̾�� ���ٸ�(���� ������ �÷��̾���)
						{
							TArray<int32> CatchedNumber = GS->GetCatchedPlayerNumber();
							
							for (int32 i = 0; i < CatchedNumber.Num(); i++)
							{
								
								if (CatchedNumber[i] == MyPS->GetTargetNumber())//���� �÷��̾� �߿� ���� Ÿ�� �÷��̾ �ִٸ�
								{
									
									TotalAddPoint += 10;
									
								}
								else
								{
									TotalAddPoint += 2;
									
								}
							}
							MyPc->ServerSetIsActiveDieSay(true);//������ �� �ִ� ���·� ����
							DieTime = false;
						}
					}

				}
				else
				{
					MyPS->SetIsDie(false);
				}

				if (!MyPS->GetIsDie())//���� �ʾҾ��ٸ�
				{
					TotalAddPoint += MyPS->GetGetPoint();
					MyPc->ServerSetGetPoint(MyPS->GetGetPoint() + 1);//getpoint�� �÷���(�� ���� �޴� ����)
					MyPc->ServerSetPlayerScore(TotalAddPoint);//������ ����
					
					MyPS->SetUpCount(5);//�������� �ٲٴ� ��� ����
					MyPS->SetMP(10);//MP����
				}

				SetCountTime(2);


				//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("player get point %d "), TotalAddPoint));
			}
			else if (IsLoadingTime3)
			{
				IsLoadingTime3 = false;
				IsMoveTime = true;

				RoundText++;//���带 �÷���
				MyPc->ServerResetChangeCost();//�������� �ٲٴ� ��� ����
				MyPc->ServerResetCatchedPlayerNumber();//������ �÷��̾�鿡 ���� ���� ����� �������Ƿ� ���� �÷��̾� ������ �ʱ�ȭ������
				MyPS->SetHP(0);
				MyPc->TargetPlayerDie();//���� Ÿ�� �÷��̾ �������� Ȯ���ϰ� ���� �߰�

				MyPc->ServerSetSortScore();//�÷��̾���� ������ �����Ͽ� �����ֱ� ����
				SetCountTime(30);//�� ���� ����

				if (RoundText == 20)//��� ���� �����
				{
					EndRound = true;
					GetWorldTimerManager().ClearTimer(CountdownTimerHandle);//Ÿ�̸� ����
				}

			}

		}
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWhyPawn::StaticClass(), FoundActors);

	for (AActor* FoundPawn : FoundActors)
	{
		AWhyPawn* WhyPawn = Cast<AWhyPawn>(FoundPawn);

		if (WhyPawn->GetStage())
		{
			InNumber++;
		}
		else
		{
			OutNumber++;
		}
	}



}

int32 AClock::GetInNumber()
{
	return InNumber;
}
int32 AClock::GetOutNumber()
{
	return OutNumber;
}

void AClock::SetCountTime(int32 CountTime)
{
	CountdownTime = CountTime;
}

int32 AClock::GetCountdownTime()
{
	return CountdownTime;
}

int32 AClock::GetRoundText()
{
	return RoundText;
}

void AClock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClock, CountdownTime);




}

bool AClock::GetIsMoveTime()
{
	return IsMoveTime;
}
bool AClock::GetIsTileChangeTime()
{
	return IsTileChangeTime;
}
int32 AClock::GetChangeCost()
{
	return ChangeCost;
}
bool AClock::GetIsLoadingTime()
{
	return IsLoadingTime;
}
bool AClock::GetIsLoadingTime2()
{
	return IsLoadingTime2;
}
bool AClock::GetEndRound()
{
	return EndRound;
}
bool AClock::GetIsLoadingTime3()
{
	return IsLoadingTime3;
}


