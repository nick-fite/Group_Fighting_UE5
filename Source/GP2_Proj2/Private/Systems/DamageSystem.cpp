// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/DamageSystem.h"

#include "ThirdPersonCharacter.h"
#include "AI/EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UDamageSystem::UDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDamageSystem::ReserveAttackToken(int amount)
{
	if(AttackTokens > 0)
	{
		AttackTokens -= amount;
		return true;
	}
	return false;
}

void UDamageSystem::ReturnAttackToken(int amount)
{
	AttackTokens += amount;
}

int UDamageSystem::GetTokenAmount()
{
	return AttackTokens;
}

void UDamageSystem::Hit()
{
	FVector endTrace = (GetOwner()->GetActorForwardVector() * 50.0) + GetOwner()->GetActorLocation();
	FHitResult hitResult;
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetOwner());

	bool hit = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	GetOwner()->GetActorLocation(), 
	endTrace,
	30,
	UEngineTypes::ConvertToTraceType(ECC_Camera),
	false,
	actorsToIgnore,
	EDrawDebugTrace::ForDuration,
	hitResult,
	true,
	FLinearColor::Red,
	FLinearColor::Blue,
	60.0f);

	if(hit)
	{
		UDamageSystem* opponentDamageSystem =
			hitResult.GetActor()->FindComponentByClass<UDamageSystem>();

		if(IsValid(opponentDamageSystem))
		{
			opponentDamageSystem->GetHit();
		}
	}
	
	
}

void UDamageSystem::GetHit()
{
	if(GetOwner()->IsA(AEnemyAI::StaticClass()))
	{
		if(Cast<AEnemyAI>(GetOwner())->HasToken)
		{
			Cast<AEnemyAI>(GetOwner())->ReturnPlayerToken();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%p"), GetOwner());

	Health--;
	if(Health > 0)
	{
		ParentMesh->GetAnimInstance()->Montage_Play(HitReaction);
	}
	else
	{
		if(GetOwner()->IsA(AEnemyAI::StaticClass()))
		{
			if(!Cast<AEnemyAI>(GetOwner())->isStunned)
			{
				Cast<AEnemyAI>(GetOwner())->isStunned = true;
				UAIBlueprintHelperLibrary::
				GetBlackboard(GetOwner())->SetValueAsBool("IsStunned", true);
			}
			else
			{
				ParentMesh->GetAnimInstance()->Montage_Play(DeathAnim);	
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("player death"));
			ParentMesh->GetAnimInstance()->Montage_Play(DeathAnim);
		}
	}
	
}
