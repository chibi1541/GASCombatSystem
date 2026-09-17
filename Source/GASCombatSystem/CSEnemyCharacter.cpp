// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEnemyCharacter.h"
#include "AbilitySystemComponent.h"


// Sets default values
ACSEnemyCharacter::ACSEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

// Called when the game starts or when spawned
void ACSEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	ASC->InitAbilityActorInfo(this, this);
}

void ACSEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
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

