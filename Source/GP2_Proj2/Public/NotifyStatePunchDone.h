// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyStatePunchDone.generated.h"

/**
 * 
 */
UCLASS()
class GP2_PROJ2_API UNotifyStatePunchDone : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
