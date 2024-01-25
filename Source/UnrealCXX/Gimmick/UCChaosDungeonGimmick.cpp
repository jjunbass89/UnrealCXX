// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/UCChaosDungeonGimmick.h"
#include "Character/UCCharacterNonPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/UCChaosDungeonModeInterface.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
AUCChaosDungeonGimmick::AUCChaosDungeonGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	OpponentSpawnTime = 2.0f;
	OpponentClass = AUCCharacterNonPlayer::StaticClass();
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

void AUCChaosDungeonGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OpponentClass, arrOutActors);
	if (bDoneSpawnOpponents && arrOutActors.Num() != MaxCurrentOponetsNum)
	{
		bDoneSpawnOpponents = false;
		OnOpponentsSpawn();
	}
}

void AUCChaosDungeonGimmick::OnOpponentsSpawn()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OpponentClass, arrOutActors);
	
	int32 NumNeedToSpawn = MaxCurrentOponetsNum - arrOutActors.Num();
	IUCChaosDungeonModeInterface* UCChaosDungeonMode = Cast<IUCChaosDungeonModeInterface>(GetWorld()->GetAuthGameMode());
	if (!bInitOpponents && UCChaosDungeonMode)
	{
		UCChaosDungeonMode->OnPlayerScoreChanged(NumNeedToSpawn);
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
		UCOpponentCharacter->OnDestroyed.AddDynamic(this, &AUCChaosDungeonGimmick::OnOpponentDestroyed);
		UCOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}
