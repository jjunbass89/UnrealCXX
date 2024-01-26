// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UCCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/UCAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Physics/UCCollision.h"

AUCCharacterNonPlayer::AUCCharacterNonPlayer()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_UCNPC);

	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AUCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/UnrealCXX/Animation/AM_AIAttack.AM_AIAttack'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/UnrealCXX/Animation/AM_AIHit.AM_AIHit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}
}

void AUCCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(MinLevel > 0);
	ensure(MaxLevel > 0);
	int32 RandIndex = FMath::RandRange(MinLevel - 1, MaxLevel - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AUCCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AUCCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AUCAIController* UCAIController = Cast<AUCAIController>(GetController());
	if (UCAIController)
	{
		UCAIController->StopAI();
	}
	
	bIsDeadAlready = true;

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AUCCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

void AUCCharacterNonPlayer::Attack()
{
	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUCCharacterNonPlayer::NotifyAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AUCCharacterNonPlayer::NotifyAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnAttackFinished.ExecuteIfBound();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AUCCharacterNonPlayer::NotifyHitEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	AUCAIController* UCAIController = Cast<AUCAIController>(GetController());
	if (UCAIController)
	{
		UCAIController->RunAI();
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AUCCharacterNonPlayer::AttackHitCheck()
{
	TSet<AActor*> OutHitActors;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRadius = 60.0f;
	const float AttackDamage = 1.0f;

	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + GetActorForwardVector() * AttackRadius * 2;

	TArray<FHitResult> OutHitResults;
	Start += GetActorForwardVector() * AttackRadius;
	End += GetActorForwardVector() * AttackRadius;
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_UCACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{	
		for (auto OutHitResult : OutHitResults)
		{
			APawn* Pawn = Cast<APawn>(OutHitResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OutHitActors.Add(OutHitResult.GetActor());
			}
		}
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), CapsuleOrigin, AttackRadius, 16, DrawColor, false, 2.0f);

#endif

	for (auto OutHitActor : OutHitActors)
	{
		FDamageEvent DamageEvent;
		OutHitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
}

float AUCCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsDeadAlready)
	{
		// Movement Setting
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		AUCAIController* UCAIController = Cast<AUCAIController>(GetController());
		if (UCAIController)
		{
			UCAIController->StopAI();
		}

		// Animation Setting
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HitMontage, 1.0);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AUCCharacterNonPlayer::NotifyHitEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, HitMontage);
	}

	return DamageAmount;
}

float AUCCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AUCCharacterNonPlayer::GetAIDetectRange()
{
	return 500.0f;
}

float AUCCharacterNonPlayer::GetAIAttackRange()
{
	float AttackRange = 40.0f, AttackRadius = 50.0f;
	return AttackRange + AttackRadius * 2;
}

float AUCCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AUCCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AUCCharacterNonPlayer::AttackByAI()
{
	Attack();
}
