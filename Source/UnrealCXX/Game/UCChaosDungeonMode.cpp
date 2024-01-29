// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UCChaosDungeonMode.h"
#include "Player/UCChaosDungeonController.h"
#include "Interface/UCChaosDungeonGameInterface.h"

AUCChaosDungeonMode::AUCChaosDungeonMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/UnrealCXX/Blueprint/BP_UCCharacterPlayer.BP_UCCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UnrealCXX.UCChaosDungeonController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	CurrentGameMode = EGameMode::CHAOS_DUNGEON;
}

void AUCChaosDungeonMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsInitialized)
	{
		bIsInitialized = true;

		IUCChaosDungeonGameInterface* ChaosDungeonGameInterface = Cast<IUCChaosDungeonGameInterface>(GetGameInstance());
		if (ChaosDungeonGameInterface)
		{
			OnPlayerScoreChanged(ChaosDungeonGameInterface->GetChaosDungeonScore());
			ChaosDungeonGameInterface->IncreaseChaosDungeonStage();
		}
	}
}

void AUCChaosDungeonMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore += NewPlayerScore;

	AUCChaosDungeonController* UCChaosDungeonController = Cast<AUCChaosDungeonController>(GetWorld()->GetFirstPlayerController());
	if (UCChaosDungeonController)
	{
		const int32 PercentageScore = FMath::Clamp<int32>((CurrentScore * 100.0f) / ClearScore, 0, 100);
		UCChaosDungeonController->GameScoreChanged(PercentageScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		if (UCChaosDungeonController)
		{
			UCChaosDungeonController->GameClear();
		}
	}
}

void AUCChaosDungeonMode::OnPlayerDead()
{
	AUCChaosDungeonController* UCChaosDungeonController = Cast<AUCChaosDungeonController>(GetWorld()->GetFirstPlayerController());
	if (UCChaosDungeonController)
	{
		UCChaosDungeonController->GameOver();
	}
}

bool AUCChaosDungeonMode::IsGameCleared()
{
	return bIsCleared;
}

void AUCChaosDungeonMode::OnInteraction()
{
	AUCChaosDungeonController* UCChaosDungeonController = Cast<AUCChaosDungeonController>(GetWorld()->GetFirstPlayerController());
	if (UCChaosDungeonController && bIsPortalActivated)
	{
		IUCChaosDungeonGameInterface* ChaosDungeonGameInterface = Cast<IUCChaosDungeonGameInterface>(GetGameInstance());
		if (ChaosDungeonGameInterface)
		{
			ChaosDungeonGameInterface->UpdateChaosDungeonScore(CurrentScore);
		}

		UCChaosDungeonController->EnterPortal();
	}
}

void AUCChaosDungeonMode::SetPortalActivate(bool bIsActivated)
{
	bIsPortalActivated = bIsActivated;
}
