// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "CSBaseAttributeSet.h"


// Sets default values
ACSEnemyCharacter::ACSEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// AttributeSet 컴포넌트 처럼 등록하기만 하면 됨. 다른 처리는 필요 없음
	BaseAttributeSet = CreateDefaultSubobject<UCSBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

// Called when the game starts or when spawned
void ACSEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (BaseAttribInitEffect)
	{
		// GameplayAbility와 마찬가지로 GamplayEffect도 원본 형태 그대로를 사용하는게 아니라 
		// FGameplayEffectSpecHandle라는 구조체로 변환 후에 ASC에 등록하고 사용합니다.
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle BaseEffectSpedHandle = ASC->MakeOutgoingSpec(BaseAttribInitEffect, 1, EffectContext);
		if (BaseEffectSpedHandle.IsValid())
		{
			// Handle을 가지고 타겟의 ASC에 Effect를 적용 (GA의 Activate 처리)
			// BaseEffectSpedHandle.Data -> TSharedPtr<UGameplayEffectSpec>
			ASC->ApplyGameplayEffectSpecToTarget(*BaseEffectSpedHandle.Data.Get(), ASC);
		}
	}
}

// Called every frame
void ACSEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACSEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ACSEnemyCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void ACSEnemyCharacter::OnAttack_Implementation()
{

}

void ACSEnemyCharacter::OnHit_Implementation()
{
	if (OnHitMontage)
	{
		// 원래는 예외처리를 해야 합니다... 애님 인스턴스가 없는 경우도 있거든요...
		GetMesh()->GetAnimInstance()->Montage_Play(OnHitMontage);
	}
}

