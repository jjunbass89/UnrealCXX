// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UCWidgetComponent.h"
#include "UCUserWidget.h"

void UUCWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUCUserWidget* UCUserWidget = Cast<UUCUserWidget>(GetWidget());
	if (UCUserWidget)
	{
		UCUserWidget->SetOwningActor(GetOwner());
	}
}

