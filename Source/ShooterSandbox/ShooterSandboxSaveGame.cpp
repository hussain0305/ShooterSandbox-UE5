// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterSandboxSaveGame.h"

UShooterSandboxSaveGame::UShooterSandboxSaveGame()
{
	gamerTag = "";
}

void UShooterSandboxSaveGame::SaveGame()
{

}

void UShooterSandboxSaveGame::LoadGame()
{

}

void UShooterSandboxSaveGame::SaveGamerTag(FString username)
{
	UShooterSandboxSaveGame* shooterSandboxSaveGame = Cast<UShooterSandboxSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSandboxSaveGame::StaticClass()));
	shooterSandboxSaveGame->gamerTag = username;
	UGameplayStatics::SaveGameToSlot(shooterSandboxSaveGame, TEXT("Slot1"), 0);
}

FString& UShooterSandboxSaveGame::GetGamerTag()
{
	UShooterSandboxSaveGame* shooterSandboxSaveGame = Cast<UShooterSandboxSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
	return shooterSandboxSaveGame->gamerTag;
}