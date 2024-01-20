// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUCPlayerController();

protected:
	virtual void BeginPlay() override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UUCHUDWidget> UCHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UUCHUDWidget> UCHUDWidget;	
};
