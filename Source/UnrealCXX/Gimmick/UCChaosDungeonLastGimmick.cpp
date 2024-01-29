// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/UCChaosDungeonLastGimmick.h"
#include "Character/UCCharacterNonPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/UCChaosDungeonModeInterface.h"
#include "Prop/UCBreakableBox.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
AUCChaosDungeonLastGimmick::AUCChaosDungeonLastGimmick()
{
	OpponentSpawnTime = 2.0f;
	OpponentClass = AUCCharacterNonPlayer::StaticClass();
	BoxClass = AUCBreakableBox::StaticClass();
}

void AUCChaosDungeonLastGimmick::BeginPlay()
{
	Super::BeginPlay();

	bInitOpponents = true;
	GetWorld()->GetTimerManager().SetTimer(OpponentsSpawnTimerHandle, this, &AUCChaosDungeonLastGimmick::OnOpponentsSpawn, OpponentSpawnTime, false);
}

// 6 | 5 | 4 
// 7 |   | 3 
// 0 | 1 | 2 
FVector AUCChaosDungeonLastGimmick::CalcRandomLocation()
{
	FVector Location = FVector(0.0f, 0.0f, 222.1f);
	switch (Phase)
	{
	case 1:
		Location += FVector(0.0f, 1540.0f, 0.0f);
		break;
	case 2:
		Location += FVector(1540.0f, 1540.0f, 0.0f);
		break;
	case 3:
		Location += FVector(1540.0f, 3080.0f, 0.0f);
		break;
	}

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

void AUCChaosDungeonLastGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	TotalDeadOpponetsNum++;

	IUCChaosDungeonModeInterface* UCChaosDungeonMode = Cast<IUCChaosDungeonModeInterface>(GetWorld()->GetAuthGameMode());
	if (UCChaosDungeonMode)
	{
		UCChaosDungeonMode->OnPlayerScoreChanged(1);
		if (UCChaosDungeonMode->IsGameCleared())
		{
			TArray<AActor*> arrOutActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), OpponentClass, arrOutActors);

			for (auto arrOutActor : arrOutActors)
			{
				arrOutActor->Destroy();
			}

			return;
		}
	}

	if ((TotalDeadOpponetsNum - PrevPhaseDeadOpponetsNum) > DeadOpponetsNumForBox)
	{
		PrevPhaseDeadOpponetsNum = TotalDeadOpponetsNum;
		Phase++;

		FVector SpawnLocation = FVector(0.0f, 0.0f, 156.0f);
		FRotator SpawnRotator;
		switch (Phase)
		{
		case 1:
			SpawnLocation += FVector(0.0f, 770.0f, 0.0f);
			SpawnRotator = FVector(0.0f, -1.0f, 0.0f).Rotation();
			break;
		case 2:
			SpawnLocation += FVector(770.0f, 1540.0f, 0.0f);
			SpawnRotator = FVector(-1.0f, 0.0f, 0.0f).Rotation();
			break;
		case 3:
			SpawnLocation += FVector(1540.0f, 2310.0f, 0.0f);
			SpawnRotator = FVector(0.0f, -1.0f, 0.0f).Rotation();
			break;
		}

		const FTransform SpawnTransform(SpawnRotator, SpawnLocation);
		AUCBreakableBox* UCBoxCharacter = GetWorld()->SpawnActorDeferred<AUCBreakableBox>(BoxClass, SpawnTransform);
		if (UCBoxCharacter)
		{
			UCBoxCharacter->OnDestroyed.AddDynamic(this, &AUCChaosDungeonLastGimmick::OnBoxDestroyed);
			UCBoxCharacter->FinishSpawning(SpawnTransform);
		}
	}
}

void AUCChaosDungeonLastGimmick::OnBoxDestroyed(AActor* DestroyedActor)
{
	OnOpponentsSpawn();
}

void AUCChaosDungeonLastGimmick::OnOpponentsSpawn()
{
	for (size_t i = 0; i < MaxCurrentOpponetsNum; i++)
	{
		const FVector SpawnLocation = CalcRandomLocation();
		const FRotator SpawnRotator = FVector(-SpawnLocation.X, -SpawnLocation.Y, 0.0f).Rotation();
		const FTransform SpawnTransform(SpawnRotator, SpawnLocation);
		AUCCharacterNonPlayer* UCOpponentCharacter = GetWorld()->SpawnActorDeferred<AUCCharacterNonPlayer>(OpponentClass, SpawnTransform);
		if (UCOpponentCharacter)
		{
			UCOpponentCharacter->SetMaxLevel(MaxLevel);
			UCOpponentCharacter->SetMinLevel(MinLevel);
			UCOpponentCharacter->OnDestroyed.AddDynamic(this, &AUCChaosDungeonLastGimmick::OnOpponentDestroyed);
			UCOpponentCharacter->FinishSpawning(SpawnTransform);
		}
	}
}