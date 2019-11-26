// Fill out your copyright notice in the Description page of Project Settings.

#include "WhyPawn.h"


// Sets default values
AWhyPawn::AWhyPawn()
{
	BaseTurnRate = 200.f;
	BaseLookUpRate = 300.f;

	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->SetupAttachment(Mesh);
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->SetWorldRotation(FRotator(-80.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Platformer/Meshes/Pl_Tree_01.Pl_Tree_01'"));
	if (StaticMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
	}

	StaticMeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//StaticMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetIsReplicated(true);
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TargetMaterial(TEXT("Material'/Game/Material/TargetColor.TargetColor'"));
	if (TargetMaterial.Succeeded())
	{
		Material = TargetMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MyMaterial(TEXT("Material'/Game/Material/MyPlayerColor.MyPlayerColor'"));
	if (MyMaterial.Succeeded())
	{
		Material1 = MyMaterial.Object;
	}

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(StaticMeshComponent);
	Collision->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	Collision->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWhyPawn::OnOverlapEvent);


	MoveToHere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoveToHereMesh"));
	MoveToHere->SetupAttachment(StaticMeshComponent);
	MoveToHere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MoveToHere->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	MoveToHere->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MOVETOHERE(TEXT("StaticMesh'/Game/Platformer/Meshes/Pl_PowerUp_02.Pl_PowerUp_02'"));
	if (MOVETOHERE.Succeeded())
	{
		MoveToHere->SetStaticMesh(MOVETOHERE.Object);
	}

	DieEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DieEffect"));
	DieEffect->SetupAttachment(StaticMeshComponent);
	DieEffect->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DIEEFFECT(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Heal_Startup_mobile.P_Heal_Startup_mobile'"));
	if (DIEEFFECT.Succeeded())
	{
		DieEffect->SetTemplate(DIEEFFECT.Object);
		DieEffect->bAutoActivate = false;
	}
	
	SpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnEffect"));
	SpawnEffect->SetupAttachment(StaticMeshComponent);
	SpawnEffect->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SPAWNEFFECT(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Health_Pickup_01.P_Health_Pickup_01'"));
	if (SPAWNEFFECT.Succeeded())
	{
		SpawnEffect->SetTemplate(SPAWNEFFECT.Object);
		SpawnEffect->bAutoActivate = false;
	}

	ShowPlayerName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerName"));
	ShowPlayerName->SetupAttachment(StaticMeshComponent);
	ShowPlayerName->SetRelativeLocation(FVector(100.0f, 0.0f, 450.0f));
	ShowPlayerName->SetWorldSize(100.0f);
	ShowPlayerName->SetRelativeRotation(FRotator(90.0f,0.0f,180.0f));
	ShowPlayerName->SetHorizontalAlignment(EHTA_Center);
	ShowPlayerName->SetTextRenderColor(FColor::Black);
	//ShowPlayerName->SetText(FText::FromString("Name"));
	//ShowPlayerName->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void AWhyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	GetPlayerAndSetColor();
	ShowPlayerName->SetText(FText::FromString(PlayerNameText));
	
}

// Called every frame
void AWhyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AWhyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AWhyPawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}


void AWhyPawn::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());


}

FVector AWhyPawn::GetMeshLocation()
{
	return StaticMeshComponent->GetComponentLocation();
}

void AWhyPawn::SetPawnLocation(int32 Rotation)
{
	if (Rotation == 1)
	{
		StaticMeshComponent->SetWorldLocation(GetMeshLocation() + FVector(100.f, 0.f, 0.f));
	}
}

bool AWhyPawn::ServerSetPawnLocation_Validate(int32 Rotation)
{
	return true;
}

void AWhyPawn::ServerSetPawnLocation_Implementation(int32 Rotation)
{
	SetMeshLocation(Location);
	
}


void AWhyPawn::SetMeshLocation(FVector Location)
{
	this->Location = Location;
	StaticMeshComponent->SetWorldLocation(Location);
	if (Role < ROLE_Authority)
	{
		ServerSetMeshLocation(Location);
	}
	
	
}

bool AWhyPawn::ServerSetMeshLocation_Validate(FVector Location)
{
	return true;
}

void AWhyPawn::ServerSetMeshLocation_Implementation(FVector Location)
{
	SetMeshLocation(Location);
	
}

void AWhyPawn::SetPlayerAndTargetNumber(int32 Num1, int32 Num2)
{
	PlayerNumber = Num1;
	TargetNumber = Num2;
	if (Role < ROLE_Authority)
	{
		ServerSetPlayerAndTargetNumber(Num1, Num2);
	}
}

bool AWhyPawn::ServerSetPlayerAndTargetNumber_Validate(int32 Num1, int32 Num2)
{
	return true;
}

void AWhyPawn::ServerSetPlayerAndTargetNumber_Implementation(int32 Num1, int32 Num2)
{
	PlayerNumber = Num1;
	TargetNumber = Num2;
}

void AWhyPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWhyPawn, Location);
	DOREPLIFETIME(AWhyPawn, PlayerNumber); 
	DOREPLIFETIME(AWhyPawn, TargetNumber);
	DOREPLIFETIME(AWhyPawn, IsWhatStage);
	DOREPLIFETIME(AWhyPawn, IsMeshVisible);
	DOREPLIFETIME(AWhyPawn, PlayerNameText);


}

void AWhyPawn::SetUpCount(int32 Value)
{
	UpCount = Value;
}
int32 AWhyPawn::GetUpCount()
{
	return UpCount;
}

void AWhyPawn::SetDownCount(int32 Value)
{
	DownCount=Value;
}
int32 AWhyPawn::GetDownCount()
{
	return DownCount;
}
int32 AWhyPawn::GetPlayerNumber()
{
	return PlayerNumber;
}


void AWhyPawn::SetPlayerNumber(int32 Num)
{
	this->PlayerNumber = Num;
	
}
void AWhyPawn::SetTargetNumber(int32 Num)
{
	this->TargetNumber = Num;
	
}

void AWhyPawn::SetTargetPlayerMaterial()
{
	StaticMeshComponent->SetMaterial(0, Material);
}

void AWhyPawn::SetMyMaterial()
{
	StaticMeshComponent->SetMaterial(0, Material1);
}

void AWhyPawn::GetPlayerAndSetColor()
{

}

void AWhyPawn::OnOverlapEvent(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
}

int32 AWhyPawn::GetTargetNumber()
{
	return TargetNumber;
}

bool AWhyPawn::GetStage()
{
	return IsWhatStage;
}
void AWhyPawn::SetStage(bool Stage)
{
	IsWhatStage = Stage;
	if (Role < ROLE_Authority)
	{
		ServerSetStage(Stage);
	}
	
}

void AWhyPawn::SetMeshVisible(bool State)
{
	StaticMeshComponent->SetVisibility(State);
}

bool AWhyPawn::ServerSetStage_Validate(bool Stage)
{
	return true;
}
void AWhyPawn::ServerSetStage_Implementation(bool Stage)
{
	IsWhatStage = Stage;
	

	AWhyPlayerState* PS = Cast<AWhyPlayerState>(PlayerState);
	if (PS)
	{
		PS->SetWhatStage(Stage);
	}
	


}

void AWhyPawn::SetMoveToHereMeshLocation(FVector Loc)
{
	MoveToHere->SetWorldLocation(Loc);
}

void AWhyPawn::GetOverlappedActor()
{
	TArray<AActor*> FoundActor;
	Collision->GetOverlappingActors(FoundActor);

	
	for (AActor* FoundActors : FoundActor)
	{
		
		AColorBox* Box = Cast<AColorBox>(FoundActors);
		if (Box)
		{
			
			SetStage(Box->IsActive());

			
		}
		
	}
	

}

void AWhyPawn::SetPlayerMeshVisible(bool Value)
{
	IsMeshVisible = Value;
	StaticMeshComponent->SetVisibility(IsMeshVisible);
	if (Role < ROLE_Authority)
	{
		ServerSetMeshVisible(Value);
	}
	if (Value == false)
	{
		MultiCastDieEffect();
	}
	else
	{
		MultiCastSpawnEffect();
	}
}
bool AWhyPawn::ServerSetMeshVisible_Validate(bool Value)
{
	return true;
}
void AWhyPawn::ServerSetMeshVisible_Implementation(bool Value)
{
	SetPlayerMeshVisible(Value);
	if (Value == false)
	{
		MultiCastDieEffect();
	}
	else
	{
		MultiCastSpawnEffect();
	}
}

void AWhyPawn::Dying()
{
	if (DieEffect != nullptr)
	{
		DieEffect->Activate(true);
	}

	SetPlayerMeshVisible(false);
}

void AWhyPawn::MultiCastDieEffect_Implementation()
{
	if (DieEffect != NULL)
	{
		
		DieEffect->Activate(true);
	}
}

void AWhyPawn::MultiCastSpawnEffect_Implementation()
{
	if (SpawnEffect != NULL)
	{
		SpawnEffect->Activate(true);
	}
}


void AWhyPawn::SetShowPlayerNameText(const FString &Name)
{
	//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("1 Player Random Name %s"), *Name));
	PlayerNameText =Name;
	//ShowPlayerName->SetText(FText::FromString(Name));
	/*if (Role < ROLE_Authority)
	{
		ServerSetPlayerNameText(Name);
	}*/

}

bool AWhyPawn::ServerSetPlayerNameText_Validate(const FString & Name)
{
	return true;
}

void AWhyPawn::ServerSetPlayerNameText_Implementation(const FString & Name)
{
	//GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Red, FString::Printf(TEXT("2 Player Random Name %s"), *Name));
	SetShowPlayerNameText(Name);
}





