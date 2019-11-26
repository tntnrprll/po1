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

	//타이머 세팅
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
	//스테이지별 있는 플레이어 수
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
			//움직이는 시간일 때
			if (IsMoveTime)
			{
				NowTime = true;
				MyPc->WhatTime = NowTime;
				SetCountTime(60);
				IsTileChangeTime = true;
				IsMoveTime = false;
			}
			//스테이지 바꾸는 시간일 때
			else if (IsTileChangeTime)
			{
				NowTime = false;
				MyPc->WhatTime = NowTime;
				IsTileChangeTime = false;
				IsLoadingTime = true;

				//위치가 변경되었다면
				if (MyPc->GetIsSetLocation())
				{
					//현재 박스와 갈 박스의 레퍼런스에서 Step를 받아와 비교해 갈수 있는지 없는지 판단하여 이동
					int32 StepSub = MyPc->GetBecomeBoxReference()->GetStep() - MyPc->GetNowBoxReference()->GetStep();

					if (StepSub < 0)
					{
						MyPS->SetMP(StepSub * 10);
					}
					else
					{
						MyPS->SetHP(StepSub * 10);
						//HP가 0이하일 경우 잡힌것으로 설정
						if (MyPS->GetHP() <= 0)
						{
							MyPS->SetIsDieTime(false);
						}
					}
					//플레이어메시의 위치를 바꿈
					MyPc->SetDestination(MyPc->GetLocation());
					//플레이어의 위치 변경이 완료되어 위치가 변하지 않았다는 상태로 변경
					MyPc->SetIsSetLocation(false);
					//현재 있는 위치의 박스를 저장
					MyPc->SetNowBox();
				}
				else
				{
					MyPc->SetDestination(MyPawn->GetMeshLocation());
				}
				


				SetCountTime(1);



			}
			//로딩시간
			else if (IsLoadingTime)
			{
				IsLoadingTime = false;
				IsLoadingTime2 = true;
				//IsMoveTime = true;



				if (!GS->GetIsActiveDieSay())//선언이 되어있는 상태라면
				{
					if (!DieTime)//선언되고 한 라운드가 지나지 않았다면
					{  }
					else//선언되고 한 라운드가 지났다면
					{
						if (MyPS->GetWhatStage() == GS->GetSayColor())//선언된 스테이지와 나의 스테이지가 같은지 확인
						{
							MyPS->SetHP(100);//죽었다는 의미에서 체력을 0으로 만들어줌
							MyPS->SetIsDieTime(false);
							MyPS->SetGetPoint(MyPS->GetGetPoint()/2);
							MyPS->SetIsDie(true);
							//MyPc->ServerSetIsActiveDieSay(true);
							MyPawn->SetPlayerMeshVisible(false);//잡혔다면 플레이어가 보이지 않도록

							MyPc->ServerAddCatchedPlayerNumber(MyPS->GetPlayerNumber());//잡힌 플레이어를 저장하는 변수에 이 잡힌 플레이어 추가

						}
						
					}

				}
				//플레이어들의 스테이지에 따라 보이고 안보이고를 설정
				MyPc->SetVisiblePlayer();

				SetCountTime(1);
				
			}
			else if (IsLoadingTime2)
			{
				IsLoadingTime2 = false;
				IsLoadingTime3 = true;
				//IsMoveTime = true;

				TotalAddPoint = 0;
				if (!GS->GetIsActiveDieSay())//선언이 되어있는 상태라면
				{
					if (!DieTime)//선언되고 한 라운드가 지나지 않았다면
					{
						DieTime = true;
						
						
					}
					else//선언되고 한 라운드가 지났다면
					{
						if (MyPS->GetPlayerNumber() == GS->GetSayPlayerNumber())//나의 번호가 선언한 플레이어와 같다면(내가 선언한 플레이어라면)
						{
							TArray<int32> CatchedNumber = GS->GetCatchedPlayerNumber();
							
							for (int32 i = 0; i < CatchedNumber.Num(); i++)
							{
								
								if (CatchedNumber[i] == MyPS->GetTargetNumber())//잡힌 플레이어 중에 나의 타깃 플레이어가 있다면
								{
									
									TotalAddPoint += 10;
									
								}
								else
								{
									TotalAddPoint += 2;
									
								}
							}
							MyPc->ServerSetIsActiveDieSay(true);//선언할 수 있는 상태로 만듦
							DieTime = false;
						}
					}

				}
				else
				{
					MyPS->SetIsDie(false);
				}

				if (!MyPS->GetIsDie())//죽지 않았었다면
				{
					TotalAddPoint += MyPS->GetGetPoint();
					MyPc->ServerSetGetPoint(MyPS->GetGetPoint() + 1);//getpoint를 늘려줌(매 라운드 받는 점수)
					MyPc->ServerSetPlayerScore(TotalAddPoint);//점수를 얻음
					
					MyPS->SetUpCount(5);//스테이지 바꾸는 비용 리셋
					MyPS->SetMP(10);//MP보충
				}

				SetCountTime(2);


				//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("player get point %d "), TotalAddPoint));
			}
			else if (IsLoadingTime3)
			{
				IsLoadingTime3 = false;
				IsMoveTime = true;

				RoundText++;//라운드를 늘려줌
				MyPc->ServerResetChangeCost();//스테이지 바꾸는 비용 리셋
				MyPc->ServerResetCatchedPlayerNumber();//잡혔던 플레이어들에 대한 점수 계산이 끝났으므로 잡힌 플레이어 변수를 초기화시켜줌
				MyPS->SetHP(0);
				MyPc->TargetPlayerDie();//나의 타깃 플레이어가 잡혔는지 확인하고 점수 추가

				MyPc->ServerSetSortScore();//플레이어들의 점수를 정렬하여 보여주기 위함
				SetCountTime(30);//한 라운드 종료

				if (RoundText == 20)//모든 라운드 종료시
				{
					EndRound = true;
					GetWorldTimerManager().ClearTimer(CountdownTimerHandle);//타이머 종료
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


