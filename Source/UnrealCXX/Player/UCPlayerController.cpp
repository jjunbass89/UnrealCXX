// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCPlayerController.h"
#include "UI/UCHUDWidget.h"

AUCPlayerController::AUCPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUCHUDWidget> UCHUDWidgetRef(TEXT("/Game/UnrealCXX/UI/WBP_HUD.WBP_HUD_C"));
	if (UCHUDWidgetRef.Class)
	{
		UCHUDWidgetClass = UCHUDWidgetRef.Class;
	}
}

void AUCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	UCHUDWidget = CreateWidget<UUCHUDWidget>(this, UCHUDWidgetClass);
	if (UCHUDWidget)
	{
		UCHUDWidget->AddToViewport();
	}
}