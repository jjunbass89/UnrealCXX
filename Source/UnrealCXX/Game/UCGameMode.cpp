// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UCGameMode.h"
#include "Interface/UCChaosDungeonGameInterface.h"

AUCGameMode::AUCGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/UnrealCXX/Blueprint/BP_UCCharacterPlayer.BP_UCCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UnrealCXX.UCPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	CurrentGameMode = EGameMode::DEFAULT;
}

void AUCGameMode::BeginPlay()
{
	Super::BeginPlay();

	IUCChaosDungeonGameInterface* ChaosDungeonGameInterface = Cast<IUCChaosDungeonGameInterface>(GetGameInstance());
	if (CurrentGameMode == EGameMode::DEFAULT && ChaosDungeonGameInterface)
	{
		ChaosDungeonGameInterface->UpdateChaosDungeonScore(0);
	}
}