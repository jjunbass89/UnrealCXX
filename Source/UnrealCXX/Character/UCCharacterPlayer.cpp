// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UCCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AUCCharacterPlayer::AUCCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(-30.0f)));
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 400.0f);
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
}

void AUCCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AUCCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUCCharacterPlayer::InputRightMouseButtonPressed);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUCCharacterPlayer::InputRightMouseButtonReleased);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUCCharacterPlayer::InputRightMouseButtonPressed);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUCCharacterPlayer::InputRightMouseButtonReleased);
}

void AUCCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bClickRightMouse)
	{
		Move();
	}
}

void AUCCharacterPlayer::InputRightMouseButtonPressed()
{
	bClickRightMouse = true;
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

void AUCCharacterPlayer::Move()
{
	FHitResult Hit;
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
	}
}