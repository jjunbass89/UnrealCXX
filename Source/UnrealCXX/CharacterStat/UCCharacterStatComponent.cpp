// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/UCCharacterStatComponent.h"

// Sets default values for this component's properties
UUCCharacterStatComponent::UUCCharacterStatComponent()
{
	MaxHp = 100.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UUCCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);	
}

float UUCCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UUCCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}

void UUCCharacterStatComponent::Revival()
{
	SetHp(MaxHp);
}
