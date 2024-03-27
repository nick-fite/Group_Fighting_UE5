// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyDeathAnimDone.generated.h"

/**
 * 
 */
UCLASS()
class GP2_PROJ2_API UNotifyDeathAnimDone : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
