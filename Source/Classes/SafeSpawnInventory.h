#pragma once

#include "UTHUDWidget.h"
#include "UTInventory.h"
#include "SafeSpawn.h"
#include "SafeSpawnInventory.generated.h"

USTRUCT()
struct FSetupInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	AUTCharacter* InvOwner;
	
	/** Sound played when the time nearly running out and the player is about to spawn */
	UPROPERTY()
	USoundBase* WarningSound;

	UPROPERTY()
	float GhostProtectionTime;
};

UCLASS()
class ASafeSpawnInventory : public AUTInventory // TODO: implement AUTTimedPowerup once engine exports it properly
{
	GENERATED_UCLASS_BODY()

	virtual void Removed() override;

	virtual void DrawInventoryHUD_Implementation(UUTHUDWidget* Widget, FVector2D Pos, FVector2D Size) override;

	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	virtual bool HUDShouldRender(UUTHUDWidget* TargetWidget);

	void SetupInventory(AUTCharacter* Other, USoundBase* InWarningSound, float InGhostProtectionTime, FUnProtectPickupDelegate UnProtectDelegate);
};