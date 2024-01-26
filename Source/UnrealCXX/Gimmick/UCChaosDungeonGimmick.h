// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UCChaosDungeonGimmick.generated.h"

UCLASS()
class UNREALCXX_API AUCChaosDungeonGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUCChaosDungeonGimmick();

// Level Section
public:
	UPROPERTY(EditAnywhere, Category = Level, Meta = (AllowPrivateAccess = "true"))
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, Category = Level, Meta = (AllowPrivateAccess = "true"))
	int32 MinLevel;

protected:
	virtual void BeginPlay() override;

	// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUCCharacterNonPlayer> OpponentClass;

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	FTimerHandle OpponentsSpawnTimerHandle;
	
	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

	void OnOpponentsSpawn();
	
	void OnOpponentSpawn();

	FVector CalcRandomLocation();

	bool bDoneSpawnOpponents = false;
	
	bool bInitOpponents = false;

	int32 MaxCurrentOpponetsNum = 20;
	
	int32 MaxTotalDeadOpponetsNum = 50;
	
	int32 DeadOpponetsNumForPortal = 35;
	
	int32 TotalDeadOpponetsNum = 0;

	// Portal Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Portal)
	TObjectPtr<class UParticleSystemComponent> Portal;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Portal, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> PortalTrigger;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Portal, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> PortalTxt;

	UFUNCTION()
	void OnPortalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPortalTriggerEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bIsPortalOverlaped = false;
};
