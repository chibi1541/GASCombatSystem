// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ComboAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComboAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GASCOMBATSYSTEM_API IComboAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Combo")
	void RemoveComboAttackBinding();
	
};
