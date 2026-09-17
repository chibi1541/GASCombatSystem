// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"

// Include 하는 헤더들은 반드시 .generated.h보다 위에 정의해야 함
#include "CSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GASCOMBATSYSTEM_API ACSPlayerCharacter : public ACharacter, public IAbilitySystemInterface
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

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Input 시에 처리되는 이벤트
	virtual void Move(const FInputActionValue& InValue);

	virtual void Look(const FInputActionValue& InValue);

	virtual void Attack();

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
	TSubclassOf<UGameplayAbility> AttackAbility;

	FGameplayAbilitySpecHandle AttackAbilityHandle;
};
