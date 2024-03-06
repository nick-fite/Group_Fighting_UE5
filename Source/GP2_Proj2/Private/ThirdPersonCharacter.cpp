// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	ViewCamera = CreateDefaultSubobject<UCameraComponent>("View Camera");

	CameraBoom->SetupAttachment(GetRootComponent());
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 800.f;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1080.f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subSystem  = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subSystem->AddMappingContext(MappingContext,0);
		}
	}
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Move);
		enhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Look);
		enhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::PlayerJump);
	}
}

void AThirdPersonCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();
	AddMovementInput(input.Y * GetMoveForwardDir() + input.X * GetMoveRightDir());
}

void AThirdPersonCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D input = InputValue.Get<FVector2D>();
	AddControllerYawInput(input.X * LookSensitivity);
	AddControllerPitchInput(-input.Y * LookSensitivity);
}

void AThirdPersonCharacter::PlayerJump()
{
	GetMesh()->GetAnimInstance()->Montage_Play(JumpMontage, .1f);
	Super::Jump();
}

FVector AThirdPersonCharacter::GetMoveForwardDir() const
{
	FVector cameraForward = ViewCamera->GetForwardVector();
	cameraForward.Z = 0;
	return cameraForward.GetSafeNormal();}

FVector AThirdPersonCharacter::GetMoveRightDir() const
{
	return ViewCamera->GetRightVector();

}
