// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CombatActorInterface.h"

#include "CSEnemyCharacter.generated.h"

class UAnimMontage;
class UCSBaseAttributeSet;
class UGameplayEffect;

UCLASS()
class GASCOMBATSYSTEM_API ACSEnemyCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	// Interface

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Inherited via ICombatActorInterface
	void OnAttack_Implementation() override;

	void OnHit_Implementation() override;

	void OnDie_Implementation() override;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UCSBaseAttributeSet> BaseAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TEMP")
	TObjectPtr<UAnimMontage> OnHitMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TEMP")
	TObjectPtr<UAnimMontage> OnDieMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> BaseAttribInitEffect;


};
