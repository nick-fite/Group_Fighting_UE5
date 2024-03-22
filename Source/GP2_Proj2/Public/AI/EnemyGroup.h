// Fill out your copyright notice in the Description page of Project Settings.f

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyGroup.generated.h"

class AEnemyAI;

UCLASS()
class GP2_PROJ2_API AEnemyGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) TSubclassOf<AEnemyAI> BPEnemyAIRef;
private:
	UPROPERTY(EditAnywhere) int EnemyCount{5};
	UPROPERTY(EditAnywhere) TArray<AEnemyAI*> EnemyList;
};
