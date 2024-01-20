// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCPlayerController.h"

void AUCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}