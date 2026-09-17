// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAnimNotify_HitTrace.h"
#include "AbilitySystemComponent.h"
#include "CSPlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CSAnimNotify_HitTrace)


void UCSAnimNotify_HitTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, HitTraceTag, FGameplayEventData());
}

FString UCSAnimNotify_HitTrace::GetNotifyName_Implementation() const
{
	return FString(TEXT("Hit Trace"));
}
