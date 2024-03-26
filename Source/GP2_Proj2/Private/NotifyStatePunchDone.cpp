// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStatePunchDone.h"

#include "AI/EnemyAI.h"

void UNotifyStatePunchDone::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* other =  MeshComp->GetOwner();
	if(AEnemyAI* AI = Cast<AEnemyAI>(other))
	{
		AI->ReturnPlayerToken();
		AI->HasToken = false;
	}
}
