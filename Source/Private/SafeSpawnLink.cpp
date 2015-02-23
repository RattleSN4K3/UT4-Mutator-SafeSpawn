#include "SafeSpawn.h"

#include "UnrealNetwork.h"

#include "SafeSpawnLink.h"

ASafeSpawnLink::ASafeSpawnLink(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.DoNotCreateDefaultSubobject(TEXT("Sprite")) )
{
	// ReplicationInfo def props
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bAlwaysRelevant = true;

	// Info def props
	bReplicateMovement = false;
	NetUpdateFrequency = 10;
}

//**********************************************************************************
// Replication
//**********************************************************************************

void ASafeSpawnLink::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASafeSpawnLink, PawnOwner);
	DOREPLIFETIME(ASafeSpawnLink, ReplicatedOriginals);
}

void ASafeSpawnLink::OnRep_PawnOwner()
{
	if (OldPawnOwner != PawnOwner) // prevent update twice 
	{
		OldPawnOwner = PawnOwner;
		if (PawnOwner != NULL) LastPawn = PawnOwner;

		UpdateGhost(PawnOwner != NULL);
	}
}

//**********************************************************************************
// Private funtions
//**********************************************************************************

void ASafeSpawnLink::UpdateGhost(bool bEnable)
{
	SetGhost(bEnable);
	SetGhostEffect(bEnable);
}

//**********************************************************************************
// Public functions
//**********************************************************************************

/** ONLY CALLED SERVERSIDED */
void ASafeSpawnLink::NotfiyNewPawn(AUTCharacter* Other)
{
	PawnOwner = Other;

	// also apply ghost effect for ded servers (due to collision etc.)
	OnRep_PawnOwner();
}

/** ONLY CALLED SERVERSIDED */
void ASafeSpawnLink::NotfiyRemove()
{
	PawnOwner = NULL;

	// also apply ghost effect for ded servers (due to collision etc.)
	OnRep_PawnOwner();
}

//**********************************************************************************
// Ghost protection funtions
//**********************************************************************************

void ASafeSpawnLink::SetGhost(bool bTurnOn)
{
	if (bTurnOn)
	{
		FGhostCollisionInfo bOriginals = FGhostCollisionInfo();
		USafeSpawn::SetGhostFor(LastPawn, true, bOriginals, false);

		if (Role == ROLE_Authority)
		{
			ReplicatedOriginals = bOriginals;
			ReplicatedOriginals.bSet = true;
		}
	}
	else if (ReplicatedOriginals.bSet)
	{
		USafeSpawn::SetGhostFor(LastPawn, false, ReplicatedOriginals, false);
	}
}

void ASafeSpawnLink::SetGhostEffect(bool bTurnOn)
{
	USafeSpawn::SetGhostEffectFor(LastPawn, bTurnOn);
}