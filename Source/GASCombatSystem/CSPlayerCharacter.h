// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "ComboAttackInterface.h"
#include "CombatActorInterface.h"

#include "NativeGameplayTags.h"

// Include 하는 헤더들은 반드시 .generated.h보다 위에 정의해야 함
#include "CSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// GameplayTag 선언
GASCOMBATSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MovingBlockTag);				// 이동 제한 Tag
GASCOMBATSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(NowAttackingTag);			// 공격 중 Tag
GASCOMBATSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(NextAttackTag);				// 다음 공격 호출 Tag
GASCOMBATSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitTraceTag);				// 공격 판정 호출 Tag


UCLASS()
class GASCOMBATSYSTEM_API ACSPlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IComboAttackInterface, public ICombatActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 컨트롤러가 빙의 되었을 때 호출되는 콜백
	virtual void PossessedBy(AController* NewController) override;

	// Interface
public:
	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Inherited via IComboAttackInterface
	void RemoveComboAttackBinding_Implementation() override;

	// Inherited via ICombatActorInterface
	void OnAttack_Implementation() override;

	void OnHit_Implementation() override;

	void OnDie_Implementation() override;

protected:
	// Input 시에 처리되는 이벤트
	virtual void Move(const FInputActionValue& InValue);

	virtual void Look(const FInputActionValue& InValue);

	virtual void Attack();

	// 추후에 점프 금지 조건을 넣기 위해 미리 오버라이드
	virtual void Jump() override;

	virtual void StartNextAttack(const FGameplayEventData* InPlayload);

	void RemoveAttackDelegate();

	// 타격 판정을 위한 함수
	virtual void HitTrace(const FGameplayEventData* InPlayload);

protected:

	// 언리얼 GC의 대상이므로 UPROPERTY를 반드시 붙혀야 함. 그렇지 않으면 사용 중에도 GC 대상이 되어서 삭제 됨 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	// GAS를 사용하기 위한 AbilitySystemComponent
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;

	// 공격 GameplayAbility
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GameplayAbilities)
	TArray<TSubclassOf<UGameplayAbility>> AttackAbilities = {};

	TArray<FGameplayAbilitySpecHandle> AttackAbilityHandles = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HitTrace)
	float CheckDistance = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HitTrace)
	float SphereRadius = 60.f;

	int32 ComboIndex = 0;
	bool bCallNextAttack = false;
	FDelegateHandle ComboDelegateHandle = {};

	FDelegateHandle HitTraceDelegateHandle = {};
};
