// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyGroup.h"
#include "AI/EnemyAI.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyGroup::AEnemyGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyGroup::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < EnemyCount; i++)
	{
		AEnemyAI* AI = Cast<AEnemyAI>(GetWorld()->SpawnActor(BPEnemyAIRef));
		AI->Group = this;
		EnemyList.Add(AI);
	}

	for(int i = 0; i < EnemyCount; i++)
	{
		for(int j = 0; j < EnemyCount; j++)
		{
			if(i != j)
			{
				EnemyList[i]->MoveIgnoreActorAdd(EnemyList[j]);	
			}
		}
	}
}

// Called every frame
void AEnemyGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
}

bool AEnemyGroup::GetEnemyLocation(FVector pointToAdd)
{
	if(EnemyLocationList.Contains(pointToAdd))
	{
		return false;
	}

	EnemyLocationList.Add(pointToAdd);
	return true;
}

FVector AEnemyGroup::GetRandomPatrolLoc()
{
	FVector newVal =
		UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(500,500,0));
	SetActorLocation(newVal);
	return  newVal;
}
