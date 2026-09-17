// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAnimNofity_ComboCancel.h"
#include "Components/SkeletalMeshComponent.h"
#include "ComboAttackInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CSAnimNofity_ComboCancel)


void UCSAnimNofity_ComboCancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}

	// ComboAttackInterface를 상속 받았다면 
	if (Owner->GetClass()->ImplementsInterface(UComboAttackInterface::StaticClass()))
	{
		// 인터페이스의 순수 가상 함수를 호출
		// 호출하는 쪽은 Actor가 무슨 타입인지 신경쓸 필요 없이 인터페이스에서 구현해야 할 기능만 호출하면 됨
		IComboAttackInterface::Execute_RemoveComboAttackBinding(Owner);
	}
}

FString UCSAnimNofity_ComboCancel::GetNotifyName_Implementation() const
{
	return TEXT("Combo Cancel");
}
