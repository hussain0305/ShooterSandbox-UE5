#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ShooterSandboxGlobal.generated.h"

UENUM(BlueprintType)
enum class EConstructType : uint8{ 
	OffensiveConstruct			UMETA(DisplayName = "Offensive Construct"),
	DefensiveConstruct			UMETA(DisplayName = "Defensive Construct"),
	UtilityConstruct			UMETA(DisplayName = "Utility Construct"),
	EnergyWeapon				UMETA(DisplayName = "Energy Weapon"),
	HandledWeapon				UMETA(DisplayName = "Handled Weapon"),
	Environmental				UMETA(DisplayName = "Environmental"),
	WallOffensiveConstruct		UMETA(DisplayName = "Wall Offensive Construct"),
	WallDefensiveConstruct		UMETA(DisplayName = "Wall Defensive Construct"),
	WallUtilityConstruct		UMETA(DisplayName = "Wall Utility Construct")
};

UENUM(BlueprintType)
enum class EMovementState : uint8 {
	Stationary	UMETA(DisplayName = "Stationary"),
	Walking		UMETA(DisplayName = "Walking"),
	Running		UMETA(DisplayName = "Running"),
	Jumping		UMETA(DisplayName = "Jumping")
};

UENUM(BlueprintType)
enum class ETurretFireMode : uint8 {
	Primary			UMETA(DisplayName = "Primary"),
	Alternate		UMETA(DisplayName = "Alternate")
};

UENUM(BlueprintType)
enum class EConstructionMode : uint8 {
	Surface			UMETA(DisplayName = "Surface"),
	Wall			UMETA(DisplayName = "Wall")
};

UENUM(BlueprintType)
enum class EHUDCommandType : uint8 {
	PlayHitAnimation			UMETA(DisplayName = "PlayHitAnimation"),
	PlayKillAnimation			UMETA(DisplayName = "PlayKillAnimation"),
	SuccessfulHit				UMETA(DisplayName = "SuccessfulHit")
};

UENUM(BlueprintType)
enum class EConstructComposition : uint8 {
	IndestructibleMesh		UMETA(DisplayName = "Indestructible, Uses Mesh"),
	DestructibleMesh		UMETA(DisplayName = "Destructible, Uses Mesh"),
	IndestructibleGeometry	UMETA(DisplayName = "Indestructible, Uses Geometry"),
	DestructibleGeometry	UMETA(DisplayName = "Destructible, Uses Geometry"),
};

UENUM(BlueprintType)
enum class EEasingFunction : uint8 {
	EaseInSine			UMETA(DisplayName = "EaseInSine"),
	EaseOutSine			UMETA(DisplayName = "EaseOutSine"),
	EaseInOutSine		UMETA(DisplayName = "EaseInOutSine"),
	EaseInQuad			UMETA(DisplayName = "EaseInQuad"),
	EaseOutQuad			UMETA(DisplayName = "EaseOutQuad"),
	EaseInOutQuad		UMETA(DisplayName = "EaseInOutQuad"),
	EaseInCubic			UMETA(DisplayName = "EaseInCubic"),
	EaseOutCubic		UMETA(DisplayName = "EaseOutCubic"),
	EaseInOutCubic		UMETA(DisplayName = "EaseInOutCubic"),
	EaseInQuart			UMETA(DisplayName = "EaseInQuart"),
	EaseOutQuart		UMETA(DisplayName = "EaseOutQuart"),
	EaseInOutQuart		UMETA(DisplayName = "EaseInOutQuart"),
	EaseInQuint			UMETA(DisplayName = "EaseInQuint"),
	EaseOutQuint		UMETA(DisplayName = "EaseOutQuint"),
	EaseInOutQuint		UMETA(DisplayName = "EaseInOutQuint"),
	EaseInExpo			UMETA(DisplayName = "EaseInExpo"),
	EaseOutExpo			UMETA(DisplayName = "EaseOutExpo"),
	EaseInOutExpo		UMETA(DisplayName = "EaseInOutExpo"),
	EaseInCirc			UMETA(DisplayName = "EaseInCirc"),
	EaseOutCirc			UMETA(DisplayName = "EaseOutCirc"),
	EaseInOutCirc		UMETA(DisplayName = "EaseInOutCirc"),
	EaseInBack			UMETA(DisplayName = "EaseInBack"),
	EaseOutBack			UMETA(DisplayName = "EaseOutBack"),
	EaseInOutBack		UMETA(DisplayName = "EaseInOutBack"),
	EaseInElastic		UMETA(DisplayName = "EaseInElastic"),
	EaseOutElastic		UMETA(DisplayName = "EaseOutElastic"),
	EaseInOutElastic	UMETA(DisplayName = "EaseInOutElastic"),
	EaseInBounce		UMETA(DisplayName = "EaseInBounce"),
	EaseOutBounce		UMETA(DisplayName = "EaseOutBounce"),
	EaseInOutBounce		UMETA(DisplayName = "EaseInOutBounce")
};

UENUM(BlueprintType)
enum class EUIElement : uint8 {
	TopBar							UMETA(DisplayName = "TopBar"),
	TopBarButton					UMETA(DisplayName = "TopBarButton"),
	TopBarButtonHighlighted			UMETA(DisplayName = "TopBarButtonHighlighted"),
	TopBarButtonText				UMETA(DisplayName = "TopBarButtonText"),
	TopBarButtonTextHighlighted		UMETA(DisplayName = "TopBarButtonTextHighlighted"),
	MenuButton						UMETA(DisplayName = "MenuButton"),
	MenuButtonHighlighted			UMETA(DisplayName = "MenuButtonHighlighted"),
	MenuButtonText					UMETA(DisplayName = "MenuButtonText"),
	MenuButtonTextHighlighted		UMETA(DisplayName = "MenuButtonTextHighlighted"),
	PanelBackgroundPrimary			UMETA(DisplayName = "PanelBackgroundPrimary"),
	PanelBackgroundSecondary		UMETA(DisplayName = "PanelBackgroundSecondary"),
	PanelBackgroundTertiary			UMETA(DisplayName = "PanelBackgroundTertiary")
};

USTRUCT(BlueprintType)
struct FConstructsDatabase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EConstructType> type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString constructName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int constructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABaseConstruct> constructBP;
};

USTRUCT(BlueprintType)
struct FColorPalette : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EUIElement> uiElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Color;
};