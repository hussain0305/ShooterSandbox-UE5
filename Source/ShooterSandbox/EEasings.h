// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterSandboxGlobal.h"

/**
 * 
 */
class SHOOTERSANDBOX_API EEasings
{
public:
	typedef double(*EasingFunction)(double);

	EasingFunction GetEasingFunction(EEasingFunction function);
};