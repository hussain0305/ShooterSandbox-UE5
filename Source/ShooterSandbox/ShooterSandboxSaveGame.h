// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/SaveGame.h"
#include "ShooterSandboxSaveGame.generated.h"

UCLASS()
class SHOOTERSANDBOX_API UShooterSandboxSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UShooterSandboxSaveGame();

/********************************
*       VARIABLES (1/2)         *
********************************/

	UPROPERTY(EditAnywhere)
	FString gamerTag;

/**************************
*       FUNCTIONS         *
**************************/

	UFUNCTION(BlueprintCallable, Category = "Save Game")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
		void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Gamer Tag")
		void SaveGamerTag(FString username);
	
	UFUNCTION(BlueprintCallable, Category = "Gamer Tag")
		FString& GetGamerTag();
};
