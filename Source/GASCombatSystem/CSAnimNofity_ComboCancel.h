// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSAnimNofity_ComboCancel.generated.h"

/**
 * 
 */
UCLASS()
class GASCOMBATSYSTEM_API UCSAnimNofity_ComboCancel : public UAnimNotify
{
	GENERATED_BODY()
	
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

};
