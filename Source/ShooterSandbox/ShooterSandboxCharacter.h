// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterSandboxCharacter.generated.h"

enum class EMovementState : uint8;
enum class EConstructionMode : uint8;

UCLASS(config=Game)
class AShooterSandboxCharacter : public ACharacter
{
	GENERATED_BODY()

/************************************
*       INHERENT PROPERTIES         *
************************************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

/****************************
*          CONSTANTS        *
****************************/

	const float GRAB_DISTANCE = 4500;
	const float JETPACK_THRUST_COST = 10;
	const float GRABBABLE_THROW_DISTANCE = 20000.f;
	
/********************************
*       VARIABLES (1/2)         *
********************************/

	//This is always set in Multicast functions, no other replication needed
	class ABaseWeapon* weaponCurrentlyHeld;
	
	EConstructionMode currentConstructionMode;

	FTimerHandle movementStateMonitoring;
	FTimerHandle jetpack;

//=#=#=#=#= EDITABLE IN BLUEPRINTS =#=#=#=#=

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasWeapon = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsFiringWeapon = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool jetpackActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float jetpackThrust = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> runCamShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> endRunCamShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> bulletHitShake;

/*******************************
*       FUNCTIONS(1/2)         *
*******************************/

//=#=#=#=#= INHERENT FUNCTIONS =#=#=#=#=

	AShooterSandboxCharacter();

	virtual void BeginPlay() override;

	void OnRep_PlayerState() override;

	void FetchPersistentComponents();

//=#=#=#=#= MOVEMENT STATE FUNCTIONS =#=#=#=#=

	void MonitorMovementState();

	void SetCurrentEMovementState(EMovementState newState);

	UFUNCTION(BlueprintCallable, Category = "Character")
	EMovementState GetCurrentEMovementState();

//=#=#=#=#= CONSTRUCTION FUNCTIONS =#=#=#=#=

	void MouseWheelDown();
	void MouseWheelUp();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleConstructionMenu();

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void SwitchConstructionMode();

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void TryConstruct(FName constructRowName);//TSubclassOf<class ABaseConstruct> construct

	UFUNCTION(BlueprintImplementableEvent, Category = "Construction")
	void TryQuickConstruct();
	
//=#=#=#=#= OFFENSIVE FUNCTIONS =#=#=#=#=

	void StartWeaponFire();
	void StopWeaponFire();
	void InitiateAlternateAction();
	void ExecuteAlternateAction();
	void WeaponAltMode();

	void AttemptControlOffensiveConstruct();

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Gameplay")
	void Client_SetOffensiveConstructInVicinity(class ABaseOffensiveConstruct* construct);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Gameplay")
	void Server_SetGrabbableInHand(class AGrabbable* grabbableInHand);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Gameplay")
	void Server_AttemptControlOffensiveConstruct(class ABaseOffensiveConstruct* constructToControl, class AShooterSandboxController* controllerController);

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Multicast_PickupOrDropWeapon(class ABaseWeapon* theWeapon);

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Client_PickupOrDropWeapon(bool hasPickedUp, const FString& weaponName);

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Client_UpdateWeaponAmmo(int max, int current);

	UFUNCTION(Server, Reliable, Category = "Gameplay")
	void Server_AttemptGrab(class AGrabbable* grabee);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	class ABaseOffensiveConstruct* GetOffensiveConstructInVicinity();

	bool GetGrabbableInVicinity(class AGrabbable* &grab);

	class AGrabbable* GetGrabbableInHand();

//=#=#=#=#= ENERGY FUNCTIONS =#=#=#=#=

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

//=#=#=#=#= ENERGY FUNCTIONS =#=#=#=#=

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Server_AddEnergy(int amount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Server_SpendEnergy(int amount, int maxEnergy);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Energy")
	void Client_UpdateEnergyOnHUD();

	UFUNCTION(Client, Unreliable)
	void Client_PlayerOutOfEnergy();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Server_PickupEnergyPack(class AEnergyPack* thePack);

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void PickupEnergyPackOnOwningClient(class AEnergyPack* thePack);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void Server_DropEnergyPack();

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "Energy")
	void DropEnergyPackOnOwningClient();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Energy")
	void Client_EnergyNotification(const FString& reason, int amount, int greenRedNeut);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Grabbable")
	void Server_FireGrabbable(FVector direction);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Grabbable")
	void Server_DropGrabbable();

//=#=#=#=#= HUD FUNCTIONS =#=#=#=#=

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Messaging System")
	void Client_SendAlertMessage(const FString& msg);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Disbalance System")
	void Client_DisbalanceBar();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Disbalance System")
	void HUD_ShowDisbalanceBar();

//=#=#=#=#= TEMP FUNCTIONS =#=#=#=#=

	UFUNCTION(BlueprintCallable)
	void PrintTestData();


protected:

/********************************
*       VARIABLES (2/2)         *
********************************/

	class AShooterSandboxGameMode* gameMode;

	bool bIsRunning;
	EMovementState previousMovementState;
	EMovementState currentMovementState;

	float walkSpeed;
	const float RUN_MULTIPLIER = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AShooterSandboxController* myController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AShooterSandboxPlayerState* myPlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AAShooterSandboxHUD* myHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class ABaseOffensiveConstruct* currentConstructInVicinity;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	class AEnergyPack* myEnergyPack;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	class AGrabbable* heldGrabbable;

/********************************
*       FUNCTIONS (2/2)         *
********************************/

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//=#=#=#=#= MOVEMENT FUNCTIONS =#=#=#=#=

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void ToggleCrouch();
	void ToggleRunOn();
	void ToggleRunOff();
	void ToggleRunCamShake(bool startShake);

	void CombinedJumpJetpackOn();
	void CombinedJumpJetpackOff();
	UFUNCTION(Server, Reliable, WithValidation)
	void Jetpack();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ToggleRun(float newSpeed);

/***********************************
*       INHERENT FUNCTIONS         *
***********************************/

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class AShooterSandboxController* GetMyController() const { return myController; }
};