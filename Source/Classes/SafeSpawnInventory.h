#pragma once

#include "SafeSpawn.h"
#include "UTHUDWidget.h"
#include "UTInventory.h"
#include "UTTimedPowerup.h"
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

UCLASS(Config = SafeSpawn)
class ASafeSpawnInventory : public AUTTimedPowerup
{
	GENERATED_UCLASS_BODY()

	//'''''''''''''''''''''''''
	// Delegates
	//'''''''''''''''''''''''''

	FUnProtectPickupDelegate UnProtectCallback;
	FTimerHandle TimerHandle_ConditionalPlayWarning;

	//'''''''''''''''''''''''''
	// Replication variables
	//'''''''''''''''''''''''''

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InvSetup)
	FSetupInfo InvSetup;
	UFUNCTION()
	virtual void OnRep_InvSetup();

	//'''''''''''''''''''''''''
	// Server variables
	//'''''''''''''''''''''''''

	UPROPERTY()
	bool bWaitForLanded;
	UPROPERTY()
	float WaitForLandedTime;

	//'''''''''''''''''''''''''
	// Client variables
	//'''''''''''''''''''''''''

	UPROPERTY()
	bool bAlreadySetup;
	UPROPERTY()
	bool bPlayWarningOnce;

	UPROPERTY()
	TArray<FWeaponRestoreInfo> WeaponRestore;

	UPROPERTY()
	bool bOriginalNoCrosshair;

	// -~= Visuals  ~=-

	UPROPERTY()
	float BarTimUsed;

	UPROPERTY()
	float FullPulseTime;
	UPROPERTY()
	bool bPulseBar;
	UPROPERTY()
	int32 PulseBarScaler;

	//'''''''''''''''''''''''''
	// Localization variables
	//'''''''''''''''''''''''''

	//var localized string ProtectionActiveMessage;  	        // Ghost protection currently active
	//var localized string ProtectionActiveMessageWithTime;  	// Ghost protection currently active (`ts)
	//var localized string FireToUnProtectMessage;  	        // Press [FIRE] to remove ghost protection
	//var localized string TimeStringMessage;			        // `ts

	//'''''''''''''''''''''''''
	// Config
	//'''''''''''''''''''''''''

	UPROPERTY(BlueprintReadWrite, Config)
	bool ShowTime;

	UPROPERTY(BlueprintReadWrite, Config)
	float BarTimeThreshold;

	UPROPERTY(BlueprintReadWrite, Config)
	bool WarningSound;


	//'''''''''''''''''''''''''
	// Inherited funtions
	//'''''''''''''''''''''''''

	virtual void TimeExpired_Implementation() override;

	virtual void Destroyed() override;
	virtual void GivenTo(AUTCharacter* NewOwner, bool bAutoActivate) override;
	virtual void Removed() override;
	virtual void OwnerEvent_Implementation(FName EventName) override;

	virtual void DrawInventoryHUD_Implementation(UUTHUDWidget* Widget, FVector2D Pos, FVector2D Size) override;

	//'''''''''''''''''''''''''
	// Private functions
	//'''''''''''''''''''''''''

	/** Serversided only */
	UFUNCTION(BlueprintAuthorityOnly)
	void AddCollisionCheck(bool add);

	/** adds or removes our bonus from the given pawn */
	UFUNCTION()
	void ClientSetup(AUTCharacter* P, bool bRemove);

	void ConditionalPlayWarning();
	void PlayWarningSound();

	UFUNCTION()
	void BlockWeapons(AUTCharacter* P, bool bBlock);

	UFUNCTION()
	void FixWeapons(AUTCharacter* P);

	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	virtual bool HUDShouldRender(UUTHUDWidget* TargetWidget);

	//UFUNCTION(BlueprintAuthorityOnly)
	void SetupInventory(AUTCharacter* Other, USoundBase* InWarningSound, float InGhostProtectionTime, FUnProtectPickupDelegate UnProtectDelegate);

	//'''''''''''''''''''''''''
	// Helper
	//'''''''''''''''''''''''''

	// TODO: Should bLocallyOwned be inline?
	// For use with listen servers
	bool bLocallyOwned()
	{
		AActor* Owner = GetUTOwner();
		if (Owner == NULL)
		{
			return false;
		}

		AController* C;
		if (Cast<APawn>(Owner) != NULL)
			C = Cast<APawn>(Owner)->GetController();
		else
			C = Cast<AController>(Owner);

		if (GWorld->GetNetMode() != NM_DedicatedServer && C != NULL && Cast<APlayerController>(C) != NULL && Cast<ULocalPlayer>(Cast<APlayerController>(C)->Player) != NULL)
			return true;

		return false;
	}

	AUTCharacter* GetUTOwner() const
	{
		if (InvSetup.InvOwner != NULL)
			return InvSetup.InvOwner;

		return Super::GetUTOwner();
	}
};