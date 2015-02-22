#include "SafeSpawn.h"

#include "UnrealNetwork.h"

#include "SafeSpawnRepInfo.h"

ASafeSpawnRepInfo::ASafeSpawnRepInfo(const FObjectInitializer& ObjectInitializer)
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


	// Inherited
	bOnlyRelevantToOwner = true;
	bAlwaysRelevant = false;

	NetPriority = 1.1f;
}

//**********************************************************************************
// Replication
//**********************************************************************************

void ASafeSpawnRepInfo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASafeSpawnRepInfo, PlayerOwner, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ASafeSpawnRepInfo, PawnCounter, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASafeSpawnRepInfo, InitialFireDelay, COND_OwnerOnly);
}

void ASafeSpawnRepInfo::OnRep_PlayerOwner()
{
	
}

void ASafeSpawnRepInfo::OnRep_PawnCounter()
{

}

//**********************************************************************************
// Public functions
//**********************************************************************************

/** CALLED SERVERSIDED ONLY */
void ASafeSpawnRepInfo::NotifyRespawned(AUTCharacter* Other)
{

}

/** CALLED SERVERSIDED ONLY */
void ASafeSpawnRepInfo::NotifyActive()
{

}