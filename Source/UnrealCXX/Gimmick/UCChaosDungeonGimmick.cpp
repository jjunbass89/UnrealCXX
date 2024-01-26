// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/UCChaosDungeonGimmick.h"
#include "Character/UCCharacterNonPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/UCChaosDungeonModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/UCCollision.h"
#include "UI/UCWidgetComponent.h"

// Sets default values
AUCChaosDungeonGimmick::AUCChaosDungeonGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	OpponentSpawnTime = 2.0f;
	OpponentClass = AUCCharacterNonPlayer::StaticClass();

	Portal = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Portal"));
	Portal->SetupAttachment(RootComponent);
	Portal->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	Portal->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PortalRef(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Summon/P_Summon_Portal.P_Summon_Portal'"));
	if (PortalRef.Object)
	{
		Portal->SetTemplate(PortalRef.Object);
		Portal->bAutoActivate = false;
	}

	MaxLevel = 2;
	MinLevel = 1;

	PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
	PortalTrigger->SetBoxExtent(FVector(120.0f, 120.0f, 150.0f));
	PortalTrigger->SetupAttachment(RootComponent);
	PortalTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 275.0f));
	PortalTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AUCChaosDungeonGimmick::OnPortalTriggerBeginOverlap);
	PortalTrigger->OnComponentEndOverlap.AddDynamic(this, &AUCChaosDungeonGimmick::OnPortalTriggerEndOverlap);

	PortalTxt = CreateDefaultSubobject<UUCWidgetComponent>(TEXT("PortalTxt"));
	PortalTxt->SetupAttachment(Portal);
	PortalTxt->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> PortalTxtRef(TEXT("/Game/UnrealCXX/UI/WBP_PortalTxt.WBP_PortalTxt_C"));
	if (PortalTxtRef.Class)
	{
		PortalTxt->SetWidgetClass(PortalTxtRef.Class);
		PortalTxt->SetWidgetSpace(EWidgetSpace::Screen);
		PortalTxt->SetDrawSize(FVector2D(100.0f, 100.0f));
		PortalTxt->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PortalTxt->SetHiddenInGame(true);
	}
}

void AUCChaosDungeonGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AUCChaosDungeonGimmick::BeginPlay()
{
	Super::BeginPlay();

	bInitOpponents = true;
	GetWorld()->GetTimerManager().SetTimer(OpponentsSpawnTimerHandle, this, &AUCChaosDungeonGimmick::OnOpponentsSpawn, OpponentSpawnTime, false);
}

// 6 | 5 | 4 
// 7 |   | 3 
// 0 | 1 | 2 
FVector AUCChaosDungeonGimmick::CalcRandomLocation()
{
	FVector Location = FVector(0.0f, 0.0f, 222.1f);
	int32 RandomIndex = FMath::RandRange(0, 7);
	switch (RandomIndex)
	{
	case 0:
		Location += FVector(-385.0f, -385.0f, 0.0f);
		break;
	case 1:
		Location += FVector(0.0f, -385.0f, 0.0f);
		break;
	case 2:
		Location += FVector(385.0f, -385.0f, 0.0f);
		break;
	case 3:
		Location += FVector(385.0f, 0.0f, 0.0f);
		break;
	case 4:
		Location += FVector(385.0f, 385.0f, 0.0f);
		break;
	case 5:
		Location += FVector(0.0f, 385.0f, 0.0f);
		break;
	case 6:
		Location += FVector(-385.0f, 385.0f, 0.0f);
		break;
	case 7:
		Location += FVector(-385.0f, 0.0f, 0.0f);
		break;
	}

	float RandomX = FMath::RandRange(-180.0f, 180.0f);
	float RandomY = FMath::RandRange(-180.0f, 180.0f);
	Location += FVector(RandomX, RandomY, 0.0f);
	return Location;
}

void AUCChaosDungeonGimmick::OnPortalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsPortalOverlaped = true;
	PortalTxt->SetHiddenInGame(!bIsPortalOverlaped);
}

void AUCChaosDungeonGimmick::OnPortalTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsPortalOverlaped = false;
	PortalTxt->SetHiddenInGame(!bIsPortalOverlaped);
}

void AUCChaosDungeonGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OpponentClass, arrOutActors);
	if (bDoneSpawnOpponents && 
		arrOutActors.Num() != MaxCurrentOpponetsNum)
	{
		bDoneSpawnOpponents = false;
		OnOpponentsSpawn();
	}
}

void AUCChaosDungeonGimmick::OnOpponentsSpawn()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OpponentClass, arrOutActors);
	
	const int32 DeadNum = bInitOpponents ? 0 : MaxCurrentOpponetsNum - arrOutActors.Num();
	TotalDeadOpponetsNum += DeadNum;
	MaxCurrentOpponetsNum = FMath::Min(MaxCurrentOpponetsNum, MaxTotalDeadOpponetsNum - TotalDeadOpponetsNum);

	const int32 NumNeedToSpawn = MaxCurrentOpponetsNum - arrOutActors.Num();
	IUCChaosDungeonModeInterface* UCChaosDungeonMode = Cast<IUCChaosDungeonModeInterface>(GetWorld()->GetAuthGameMode());
	if (!bInitOpponents && UCChaosDungeonMode)
	{
		UCChaosDungeonMode->OnPlayerScoreChanged(DeadNum);
		if (UCChaosDungeonMode->IsGameCleared())
		{
			for (auto arrOutActor : arrOutActors)
			{
				arrOutActor->Destroy();
			}
			
			return;
		}
	}

	for (size_t i = 0; i < NumNeedToSpawn; i++)
	{
		OnOpponentSpawn();
	}

	if (TotalDeadOpponetsNum >= DeadOpponetsNumForPortal)
	{
		Portal->Activate(true);
		PortalTrigger->SetCollisionProfileName(CPROFILE_UCTRIGGER);
	}

	bDoneSpawnOpponents = true;

	if (bInitOpponents)
	{
		bInitOpponents = false;
	}
}

void AUCChaosDungeonGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = CalcRandomLocation();
	const FRotator SpawnRotator = FVector(-SpawnLocation.X, -SpawnLocation.Y, 0.0f).Rotation();
	const FTransform SpawnTransform(SpawnRotator, SpawnLocation);
	AUCCharacterNonPlayer* UCOpponentCharacter = GetWorld()->SpawnActorDeferred<AUCCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (UCOpponentCharacter)
	{
		UCOpponentCharacter->SetMaxLevel(MaxLevel);
		UCOpponentCharacter->SetMinLevel(MinLevel);
		UCOpponentCharacter->OnDestroyed.AddDynamic(this, &AUCChaosDungeonGimmick::OnOpponentDestroyed);
		UCOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}
