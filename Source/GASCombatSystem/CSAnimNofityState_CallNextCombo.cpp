// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAnimNofityState_CallNextCombo.h"
#include "ComboAttackInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CSPlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CSAnimNofityState_CallNextCombo)

void UCSAnimNofityState_CallNextCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();

	if (IsValid(Owner) == false)
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, NextAttackTag, FGameplayEventData());
}
