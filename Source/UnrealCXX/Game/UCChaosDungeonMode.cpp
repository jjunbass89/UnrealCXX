// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UCChaosDungeonMode.h"
#include "Player/UCChaosDungeonController.h"

AUCChaosDungeonMode::AUCChaosDungeonMode()
{
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

	ClearScore = 150;
	CurrentScore = 0;
	bIsCleared = false;
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
