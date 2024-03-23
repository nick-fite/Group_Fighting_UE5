// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/DamageSystem.h"

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageSystem = CreateDefaultSubobject<UDamageSystem>("Damage System");
	
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

	AnimInstance = GetMesh()->GetAnimInstance();
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
		enhancedInputComp->BindAction(PunchAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::PlayerPunch);

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
	if(!AnimInstance->Montage_IsPlaying(nullptr))
	{
		AnimInstance->Montage_Play(JumpMontage,  1.0f);
	}
		Super::Jump();
}

void AThirdPersonCharacter::PlayerPunch()
{
	if(!AnimInstance->Montage_IsPlaying(nullptr))
	{
		const int sectionToPlay = FMath::RandRange(0,6);
		const float playRate = 2.0f;
		
		FName sectionName;
		switch (sectionToPlay)
		{
		case 0:
			sectionName = "punch1";
			break;
		case 1:
			sectionName = "punch2";
			break;
		case 2:
			sectionName = "punch3";
			break;
		case 3:
			sectionName = "punch4";
			break;
		case 4:
			sectionName = "punch5";
			break;
		case 5:
			sectionName = "punch6";
			break;
			default:
				sectionName = "punch1";
				break;
		}
		
		AnimInstance->Montage_Play(PunchMontage,  playRate);	
		AnimInstance->Montage_JumpToSection(sectionName);

		SetActorRotation(GetMoveForwardDir().Rotation());
	}
}

FVector AThirdPersonCharacter::GetMoveForwardDir() const
{
	FVector cameraForward = ViewCamera->GetForwardVector();
	cameraForward.Z = 0;
	return cameraForward.GetSafeNormal();
}

FVector AThirdPersonCharacter::GetMoveRightDir() const
{
	return ViewCamera->GetRightVector();

}
