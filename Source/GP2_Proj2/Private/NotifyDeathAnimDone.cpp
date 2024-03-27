// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyDeathAnimDone.h"

#include "AI/EnemyAI.h"

void UNotifyDeathAnimDone::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* other =  MeshComp->GetOwner();

	if(AEnemyAI* AI = Cast<AEnemyAI>(other))
	{
			AI->GetMesh()->SetSimulatePhysics(true);
	}
}
