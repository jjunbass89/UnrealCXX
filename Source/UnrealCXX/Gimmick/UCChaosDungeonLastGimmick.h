// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UCChaosDungeonLastGimmick.generated.h"

UCLASS()
class UNREALCXX_API AUCChaosDungeonLastGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUCChaosDungeonLastGimmick();

	// Level Section
public:
	UPROPERTY(EditAnywhere, Category = Level, Meta = (AllowPrivateAccess = "true"))
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, Category = Level, Meta = (AllowPrivateAccess = "true"))
	int32 MinLevel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUCCharacterNonPlayer> OpponentClass;

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUCBreakableBox> BoxClass;

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	FTimerHandle OpponentsSpawnTimerHandle;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnBoxDestroyed(AActor* DestroyedActor);

	void OnOpponentsSpawn();

	FVector CalcRandomLocation();

	bool bDoneSpawnOpponents = false;

	bool bInitOpponents = false;

	int32 MaxCurrentOpponetsNum = 30;

	int32 MaxTotalDeadOpponetsNum = 100;

	int32 DeadOpponetsNumForBox = 20;

	int32 TotalDeadOpponetsNum = 0;
	
	int32 PrevPhaseDeadOpponetsNum = 0;

	int32 Phase = 0;
};
