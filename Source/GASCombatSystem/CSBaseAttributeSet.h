// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CSBaseAttributeSet.generated.h"

// AttributeSet의 각종 Getter, Setter를 빠르게 선언하기 위한 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GASCOMBATSYSTEM_API UCSBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData BaseMoveSpeed;
	ATTRIBUTE_ACCESSORS(UCSBaseAttributeSet, BaseMoveSpeed)
	
	// Attribute의 변경이 확정되기 전 클램프 처리. HP와 Stamina가 Max치 보다 높거나 0보다 작아지는 걸 막는 처리를 진행
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// GameplayEffect가 적용된 후처리
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


};
