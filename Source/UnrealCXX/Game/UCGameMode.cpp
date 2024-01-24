// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UCGameMode.h"

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
