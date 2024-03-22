// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAI.generated.h"

class AThirdPersonCharacter;
class UGameplayStatics;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKMovingEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKPunchingEnded);

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
		TSubclassOf<AThirdPersonCharacter> classToFind;
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, AllActors);
		PlayerActor = AllActors[0];
	}

	UFUNCTION(BlueprintCallable)
	void SetDistanceToPlayerAuto()
	{
		DistanceToPlayer = FVector::Dist(this->GetActorLocation(), PlayerActor->GetActorLocation());
	}

	UFUNCTION(BlueprintCallable) void PunchPlayer();
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable) FKMovingEnded OnMovingEnded;
	UPROPERTY(BlueprintCallable, BlueprintAssignable) FKPunchingEnded OnPunchingEnded;
private:
	UFUNCTION(BlueprintCallable) void UpdateWalkSpeed(float newVal);

	

	UPROPERTY(EditAnywhere) float MaxWalkSpeed{500.0f};
	
	UPROPERTY(EditAnywhere) AActor* PlayerActor;
	UPROPERTY(EditAnywhere) float DistanceToPlayer;

	UPROPERTY(EditAnywhere) UAnimMontage* PunchMontage;
};
