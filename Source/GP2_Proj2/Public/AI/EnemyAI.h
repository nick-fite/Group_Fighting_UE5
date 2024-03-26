// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAI.generated.h"

class AEnemyGroup;
class AThirdPersonCharacter;
class UGameplayStatics;
class UDamageSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKMovingEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKPunchingEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKAskToAttack);

UCLASS()
class GP2_PROJ2_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetDistanceToPlayer()
	{
		return DistanceToPlayer;
	}

	UFUNCTION(BlueprintCallable)
	AActor* GetPlayer()
	{
		return PlayerActor;
	}

	UFUNCTION(BlueprintCallable)
	void SetPlayerActor(AActor* newPlayerActor)
	{
		PlayerActor = newPlayerActor;
	}

	UFUNCTION(BlueprintCallable)
	void SetDistanceToPlayer(float newDistance)
	{
		DistanceToPlayer = newDistance;
	}

	UFUNCTION(BlueprintCallable)
	void SetPlayerActorAuto()
	{
		PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		UE_LOG(LogTemp, Warning, TEXT("Player: %p was found"), PlayerActor);
	}

	UFUNCTION(BlueprintCallable)
	void SetDistanceToPlayerAuto()
	{
		DistanceToPlayer = FVector::Dist(this->GetActorLocation(), PlayerActor->GetActorLocation());
	}

	UFUNCTION(BlueprintCallable) void PunchPlayer();
	
	UFUNCTION(BlueprintCallable) bool AskToAttack();
	UFUNCTION(BlueprintCallable) void ReturnPlayerToken();

	UFUNCTION(BlueprintCallable) void LookAtPlayer();

	UPROPERTY(BlueprintCallable, BlueprintAssignable) FKMovingEnded OnMovingEnded;
	UPROPERTY(BlueprintCallable, BlueprintAssignable) FKPunchingEnded OnPunchingEnded;
	UPROPERTY(BlueprintCallable, BlueprintAssignable) FKAskToAttack OnAskToAttack;

	UPROPERTY() AEnemyGroup* Group;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasToken;
private:
	UFUNCTION(BlueprintCallable) void UpdateWalkSpeed(float newVal);

	UPROPERTY(EditAnywhere) float MaxWalkSpeed{500.0f};
	
	UPROPERTY(EditAnywhere) AActor* PlayerActor;
	UPROPERTY(EditAnywhere) float DistanceToPlayer;

	UPROPERTY(EditAnywhere) UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere) UDamageSystem* DamageSystem;

	UFUNCTION() void SetAskAttackBlackBoardValue();
};
