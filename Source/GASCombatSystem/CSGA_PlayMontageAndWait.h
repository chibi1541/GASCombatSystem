// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CSGA_PlayMontageAndWait.generated.h"

/**
 * 
 */
class UAnimMontage;

UCLASS()
class GASCOMBATSYSTEM_API UCSGA_PlayMontageAndWait : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCSGA_PlayMontageAndWait();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnMontageCompleted();
	virtual void OnMontageCompleted_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnMontageInterrupted();
	virtual void OnMontageInterrupted_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnMontageCancelled();
	virtual void OnMontageCancelled_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnMontageBlendOut();
	virtual void OnMontageBlendOut_Implementation();

protected:
	// Ability 실행 시에 재생할 AnimMontage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> PlayMontage;
};
