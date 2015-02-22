#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTCharacter.h"
//#include "UTPlayerController.h"

#include "SafeSpawnLink.generated.h"

UCLASS(Blueprintable)
class ASafeSpawnLink : public AInfo
{
	GENERATED_UCLASS_BODY()

protected:

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
	float ReplicatedOriginals;

	//'''''''''''''''''''''''''
	// Public functions
	//'''''''''''''''''''''''''

public:

	/** ONLY CALLED SERVERSIDED */
	void NotfiyNewPawn(AUTCharacter* Other);

	/** ONLY CALLED SERVERSIDED */
	void NotfiyRemove();
};
