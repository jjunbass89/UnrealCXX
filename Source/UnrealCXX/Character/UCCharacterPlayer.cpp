// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UCCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UCComboActionData.h"
#include "FX/UCCircleRing.h"

const float AUCCharacterPlayer::MaxTargetArmLength = 1200.0f;
const float AUCCharacterPlayer::MinTargetArmLength = 220.0f;
const float AUCCharacterPlayer::MaxSocketOffset = 600.0f;
const float AUCCharacterPlayer::MinSocketOffset = 110.0f;

AUCCharacterPlayer::AUCCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(30.0f)));
	CameraBoom->TargetArmLength = MaxTargetArmLength;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, MaxSocketOffset);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(30.0f)));
	FollowCamera->bUsePawnControlRotation = false;
	
	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/UnrealCXX/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UnrealCXX/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UnrealCXX/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UnrealCXX/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputZoomInRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UnrealCXX/Input/Actions/IA_ZoomIn.IA_ZoomIn'"));
	if (nullptr != InputZoomInRef.Object)
	{
		ZoomIn = InputZoomInRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputZoomOutRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UnrealCXX/Input/Actions/IA_ZoomOut.IA_ZoomOut'"));
	if (nullptr != InputZoomOutRef.Object)
	{
		ZoomOut = InputZoomOutRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/UnrealCXX/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UUCComboActionData> ComboActionDataRef(TEXT("/Script/UnrealCXX.UCComboActionData'/Game/UnrealCXX/CharacterAction/AUC_ComboAttack.AUC_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Baguette/SK_Blade_Baguette.SK_Blade_Baguette'"));
	if (WeaponMeshRef.Object)
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
		Weapon->SetSkeletalMesh(WeaponMeshRef.Object);
		Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	}
	
	CircleRingClass = AUCCircleRing::StaticClass();
	
	bClickRightMouse = false;
	bClickLeftMouse = false;
	bIsZoomingIn = false;
	bIsZoomingOut = false;
	ZoomFactor = 0.0f;
}

void AUCCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FVector WorldSpawnLocation = GetActorLocation();
	AActor* CircleRingActor = GetWorld()->SpawnActor(CircleRingClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
	CircleRing = Cast<AUCCircleRing>(CircleRingActor);
	CircleRing->SetActorHiddenInGame(true);
}

void AUCCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AUCCharacterPlayer::InputRightMouseButtonPressed);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUCCharacterPlayer::InputRightMouseButtonReleased);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AUCCharacterPlayer::InputLeftMouseButtonPressed);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AUCCharacterPlayer::InputLeftMouseButtonReleased);
	EnhancedInputComponent->BindAction(ZoomIn, ETriggerEvent::Started, this, &AUCCharacterPlayer::InputMouseWheelUp);
	EnhancedInputComponent->BindAction(ZoomOut, ETriggerEvent::Started, this, &AUCCharacterPlayer::InputMouseWheelDown);
}

void AUCCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bClickRightMouse)
	{
		Move();
	}

	if (bNewDestinationSet)
	{
		// Hide FX when player arrive at the destination.
		const float distance = FVector::Dist(NewDestination, GetActorLocation());
		if (distance < 120.0f)
		{
			CircleRing->SetActorHiddenInGame(true);
		}
	}

	if (bClickLeftMouse)
	{
		// Initializes the target location when the attack begins.
		const float distance = FVector::Dist(NewDestination, GetActorLocation());
		if (distance > 120.0f)
		{
			NewDestination = GetActorLocation();
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), GetActorLocation());
		}

		RotationToCursor();
		Attack();
	}

	if (bIsZoomingIn)
	{
		ZoomFactor += DeltaTime / 2.0f;
		CameraBoom->TargetArmLength = FMath::Lerp<float>(MaxTargetArmLength, MinTargetArmLength, ZoomFactor);
		CameraBoom->SocketOffset = FVector(0.0f, 0.0f, FMath::Lerp<float>(MaxSocketOffset, MinSocketOffset, ZoomFactor));
		if (ZoomFactor > 1.0f)
		{
			bIsZoomingIn = false;
			CameraBoom->TargetArmLength = MinTargetArmLength;
			CameraBoom->SocketOffset = FVector(0.0f, 0.0f, MinSocketOffset);
		}
	}

	if (bIsZoomingOut)
	{
		ZoomFactor -= DeltaTime / 2.0f;
		CameraBoom->TargetArmLength = FMath::Lerp<float>(MaxTargetArmLength, MinTargetArmLength, ZoomFactor);
		CameraBoom->SocketOffset = FVector(0.0f, 0.0f, FMath::Lerp<float>(MaxSocketOffset, MinSocketOffset, ZoomFactor));
		if (ZoomFactor < 0.0f)
		{
			bIsZoomingOut = false;
			CameraBoom->TargetArmLength = MaxTargetArmLength;
			CameraBoom->SocketOffset = FVector(0.0f, 0.0f, MaxSocketOffset);
		}
	}
}

void AUCCharacterPlayer::InputRightMouseButtonPressed()
{
	bClickRightMouse = true;
	bNewDestinationSet = false;

	// Once it starts moving, stop the combo.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.1f);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
	
void AUCCharacterPlayer::InputRightMouseButtonReleased()
{
	bClickRightMouse = false;
}

void AUCCharacterPlayer::SetNewDestination(const FVector Destination)
{
	const float distance = FVector::Dist(Destination, GetActorLocation());
	if (distance > 120.0f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Destination);
	}
}

void AUCCharacterPlayer::UpdateCircleRing(const FVector Destination)
{
	const float distance = FVector::Dist(NewDestination, Destination);
	if (!bNewDestinationSet)
	{
		bNewDestinationSet = true;
		if (distance > 120.0f)
		{
			NewDestination = FVector(Destination.X, Destination.Y, Destination.Z + 1.0f);

			CircleRing->SetActorLocation(NewDestination);
			CircleRing->SetActorHiddenInGame(false);
		}
	}
	else if (distance > 120.0f)
	{
		CircleRing->SetActorHiddenInGame(true);
	}
}

void AUCCharacterPlayer::InputMouseWheelUp()
{
	bIsZoomingIn = true;
	bIsZoomingOut = false;
}

void AUCCharacterPlayer::InputMouseWheelDown()
{
	bIsZoomingOut = true;
	bIsZoomingIn = false;
}

void AUCCharacterPlayer::Move()
{
	FHitResult Hit;
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
		UpdateCircleRing(Hit.ImpactPoint);
	}
}

void AUCCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void AUCCharacterPlayer::InputLeftMouseButtonPressed()
{
	bClickLeftMouse = true;
	CurrentCombo = 0;
}

void AUCCharacterPlayer::InputLeftMouseButtonReleased()
{
	bClickLeftMouse = false;
}

void AUCCharacterPlayer::ProcessComboCommand()
{
	if (CurrentCombo == ComboActionData->MaxComboCount)
	{
		CurrentCombo = 0;
	}

	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AUCCharacterPlayer::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AUCCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AUCCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AUCCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AUCCharacterPlayer::ComboCheck, ComboEffectiveTime, false);
	}
}

void AUCCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AUCCharacterPlayer::RotationToCursor()
{
	FHitResult Hit;

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit)
	{
		const FVector headingVector = Hit.ImpactPoint - GetActorLocation();
		const FRotator targetRotator = FVector(headingVector.X, headingVector.Y, 0.0f).Rotation();
		SetActorRotation(targetRotator);
	}
}
