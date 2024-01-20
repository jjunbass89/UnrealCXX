// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UCUserWidget.h"
#include "UCHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API UUCHpBarWidget : public UUCUserWidget
{
	GENERATED_BODY()
	
public:
	UUCHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
