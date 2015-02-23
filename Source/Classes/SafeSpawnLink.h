#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTCharacter.h"
//#include "UTPlayerController.h"

#include "SafeSpawn.h"
#include "SafeSpawnLink.generated.h"

UCLASS(Blueprintable)
class ASafeSpawnLink : public AInfo
{
	GENERATED_UCLASS_BODY()

protected:

	//'''''''''''''''''''''''''
	// Workflow variables
	//'''''''''''''''''''''''''
	
	FUnProtectFireDelegate UnProtectCallback;
	
	//'''''''''''''''''''''''''
	// Server variables
	//'''''''''''''''''''''''''

	AUTCharacter* LastPawn;
	AUTCharacter* OldPawnOwner;

	//'''''''''''''''''''''''''
	// Replication variables
	//'''''''''''''''''''''''''

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_PawnOwner)
	AUTCharacter* PawnOwner;
	UFUNCTION()
	virtual void OnRep_PawnOwner();

	UPROPERTY(Replicated)
	FGhostCollisionInfo ReplicatedOriginals;

	//'''''''''''''''''''''''''
	// Private functions
	//'''''''''''''''''''''''''

	void UpdateGhost(bool bEnable);

	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	/** ONLY CALLED SERVERSIDED */
	void NotfiyNewPawn(AUTCharacter* Other);

	/** ONLY CALLED SERVERSIDED */
	void NotfiyRemove();

	//'''''''''''''''''''''''''
	// Ghost protection funtions
	//'''''''''''''''''''''''''

	void SetGhost(bool bTurnOn);
	void SetGhostEffect(bool bTurnOn);

};
