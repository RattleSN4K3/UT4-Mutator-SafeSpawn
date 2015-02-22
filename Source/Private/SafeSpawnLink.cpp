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

void ASafeSpawnLink::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASafeSpawnLink, PawnOwner);
	DOREPLIFETIME(ASafeSpawnLink, ReplicatedOriginals);
}

void ASafeSpawnLink::OnRep_PawnOwner()
{

}