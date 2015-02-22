#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTCharacter.h"
//#include "UTPlayerController.h"

#include "SafeSpawn.h"

#include "SafeSpawnRepInfo.generated.h"

UCLASS(Blueprintable)
class ASafeSpawnRepInfo : public AInfo
{
	GENERATED_UCLASS_BODY()

	//'''''''''''''''''''''''''
	// Delegates
	//'''''''''''''''''''''''''

	FUnProtectFireDelegate UnProtectCallback;

	//'''''''''''''''''''''''''
	// Init funtions
	//'''''''''''''''''''''''''

	void InitialSetup(APlayerController* PC, FUnProtectFireDelegate UnProtectDelegate, float InInitialFireDelay);
	void SetupInteraction(APlayerController* PC, bool bAdd);


protected:

	//'''''''''''''''''''''''''
	// Server variables
	//'''''''''''''''''''''''''

	UPROPERTY()
	bool bFireReceived;

	//'''''''''''''''''''''''''
	// Replication variables
	//'''''''''''''''''''''''''

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PlayerOwner)
	APlayerController* PlayerOwner;
	UFUNCTION()
	virtual void OnRep_PlayerOwner();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PawnCounter)
	AUTCharacter* PawnCounter;
	UFUNCTION()
	virtual void OnRep_PawnCounter();

	UPROPERTY(Replicated)
	float InitialFireDelay;

	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	/** CALLED SERVERSIDED ONLY */
	void NotifyRespawned(AUTCharacter* Other);

	/** CALLED SERVERSIDED ONLY */
	void NotifyActive();

};