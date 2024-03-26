// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/DamageSystem.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageSystem = CreateDefaultSubobject<UDamageSystem>("Damage System");
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	SetPlayerActorAuto();

	OnAskToAttack.AddDynamic(this, &AEnemyAI::SetAskAttackBlackBoardValue);
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetDistanceToPlayer() < 100.0f)
	{
		OnMovingEnded.Broadcast();
	}

	if(!GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		OnPunchingEnded.Broadcast();
	}
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAI::PunchPlayer()
{
	if(!GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		const int sectionToPlay = FMath::RandRange(0,6);
		const float playRate = 1.0f;
		
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
		
		GetMesh()->GetAnimInstance()->Montage_Play(PunchMontage,  playRate);	
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(sectionName);
	}
}

bool AEnemyAI::AskToAttack()
{
	AThirdPersonCharacter* tpcPlayer = Cast<AThirdPersonCharacter>(PlayerActor);
	UE_LOG(LogTemp, Warning, TEXT("tokens available: %d"), tpcPlayer->DamageSystem->GetTokenAmount());
	if( tpcPlayer->DamageSystem->ReserveAttackToken(1))
	{
		HasToken = true;
	}
	return  HasToken;
}

void AEnemyAI::ReturnPlayerToken()
{
	AThirdPersonCharacter* tpcPlayer = Cast<AThirdPersonCharacter>(PlayerActor);
	tpcPlayer->DamageSystem->ReturnAttackToken(1);
}

void AEnemyAI::LookAtPlayer()
{
	FRotator newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerActor->GetActorLocation());

	SetActorRotation(newRot);
}

void AEnemyAI::UpdateWalkSpeed(float newVal)
{
	MaxWalkSpeed = newVal;
}

void AEnemyAI::SetAskAttackBlackBoardValue()
{
	UAIBlueprintHelperLibrary::GetBlackboard(this)->SetValueAsBool("HasAttackToken",HasToken);
}

