// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCPlayerController.h"

AUCPlayerController::AUCPlayerController()
{
}

void AUCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}