// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UCCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/UCCharacterAIInterface.h"
#include "UCCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=UnrealCXX)
class UNREALCXX_API AUCCharacterNonPlayer : public AUCCharacterBase, public IUCCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AUCCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;	
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;
	
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

	// Attack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	float AttackSpeedRate = 1.0f;

	void Attack();

	void NotifyAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void AttackHitCheck() override;

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;
};
