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
	SetOwner(PlayerOwner);
	SetupInteraction(PlayerOwner, true);
}

void ASafeSpawnRepInfo::OnRep_PawnCounter()
{
#if UE_BUILD_DEBUG

	UE_LOG(LogTemp, Log, TEXT("OnRep_PawnCounter"));
	if (PlayerOwner == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("OnRep_PawnCounter - No Pawn Owner!!!"));
	}

#endif

	checkSlow(PlayerOwner != NULL);
	if (PawnCounter != NULL && PawnCounter != OldPawn &&

		// check if we are watching our own Pawn
		PlayerOwner->GetViewTarget() == PawnCounter)
	{
		OldPawn = PawnCounter;
		PawnCounterTime = GetWorld()->GetRealTimeSeconds();

		UpdateGhostFor(PawnCounter, true);
	}
}

//**********************************************************************************
// Init functions
//**********************************************************************************

void ASafeSpawnRepInfo::InitialSetup(APlayerController* PC, FUnProtectFireDelegate UnProtectDelegate, float InInitialFireDelay)
{
	PlayerOwner = PC;
	UnProtectCallback = UnProtectDelegate;
	InitialFireDelay = InInitialFireDelay;

	// for listen servers
	if (GWorld->GetNetMode() != NM_DedicatedServer && bLocallyOwned())
	{
		OnRep_PlayerOwner();
	}
}

void ASafeSpawnRepInfo::SetupInteraction(APlayerController* PC, bool bAdd)
{
	// TODO: find a suited method to interact user keybindings
}

//**********************************************************************************
// Delegate Callbacks
//**********************************************************************************

void ASafeSpawnRepInfo::OnFireInput()
{
	if (bFireCalled || USafeSpawn::ShouldIgnoreInputForNow(PawnCounterTime, GetWorld()->GetRealTimeSeconds()))
	{
		USafeSpawn::PlayFireBlockedWarningFor(PlayerOwner);
		return;
	}

	ServerFired();
	bFireCalled = true;
}

//**********************************************************************************
// Client funtions
//**********************************************************************************

/** called on the owning client just before the pickup is dropped or destroyed */
void ASafeSpawnRepInfo::ClientSetActive_Implementation()
{
	UpdateGhostFor(OldPawn, false);
}

void ASafeSpawnRepInfo::ClientUnblockInput_Implementation()
{
	bFireCalled = false;
	//ClientInteraction.BlockInput(false);
}

//**********************************************************************************
// Server funtions
//**********************************************************************************

bool ASafeSpawnRepInfo::ServerFired_Validate()
{
	return true;
}

/** called on the owning client just before the pickup is dropped or destroyed */
void ASafeSpawnRepInfo::ServerFired_Implementation()
{
	if (!bFireReceived && UnProtectCallback.IsBound())
	{
		bFireReceived = true;

		UnProtectCallback.Execute(PlayerOwner, this);
	}
}

//**********************************************************************************
// Public functions
//**********************************************************************************

/** CALLED SERVERSIDED ONLY */
void ASafeSpawnRepInfo::NotifyRespawned(AUTCharacter* Other)
{
	PawnCounter = Other;

	bFireReceived = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_StopBlockingFire);

	// for listen servers
	if (GWorld->GetNetMode() != NM_DedicatedServer && bLocallyOwned())
	{
		OnRep_PawnCounter();
	}
}

/** CALLED SERVERSIDED ONLY */
void ASafeSpawnRepInfo::NotifyActive()
{
	ClientSetActive();
	
	GetWorldTimerManager().SetTimer(TimerHandle_StopBlockingFire, this, &ASafeSpawnRepInfo::StopBlockingFire, FMath::Max(0.001f, InitialFireDelay), false);
}

//**********************************************************************************
// Timed funtions
//**********************************************************************************

void ASafeSpawnRepInfo::StopBlockingFire()
{
	ClientUnblockInput();
}

//**********************************************************************************
// Private functions
//**********************************************************************************

void ASafeSpawnRepInfo::UpdateGhostFor(AUTCharacter* P, bool bEnable)
{
	SetThirdPerson(PlayerOwner, bEnable);
	SetPPEffects(PlayerOwner, bEnable);

	if (bEnable)
	{
		//ClientInteraction.BlockInput(true);
	}
}

//**********************************************************************************
// Ghost protection funtions
//**********************************************************************************

/** applies and removes any post processing effects while holding this item */
void ASafeSpawnRepInfo::SetPPEffects(APlayerController* PC, bool bAdd)
{
	if (PC == NULL)
		return;

	USafeSpawn::SetPPEffectsFor(PC, bAdd, bOriginalOverridePostProcessSettings, OriginalPostProcessSettingsOverride, PP_Scene_Changed);
}

void ASafeSpawnRepInfo::SetGhost(AUTCharacter* P, bool bTurnOn)
{
	if (P == NULL)
		return;

	FGhostCollisionInfo bOriginals;
	bOriginals.bOriginalCollideActors = bOriginalCollideActors;
	bOriginals.bOriginalBlockActors = bOriginalBlockActors;
	bOriginals.bOriginalPushesRigidBodies = bOriginalPushesRigidBodies;
	bOriginals.bOriginalIgnoreForces = bOriginalIgnoreForces;

	USafeSpawn::SetGhostFor(P, bTurnOn, bOriginals, false);

	bOriginalCollideActors = bOriginals.bOriginalCollideActors;
	bOriginalBlockActors = bOriginals.bOriginalBlockActors;
	bOriginalPushesRigidBodies = bOriginals.bOriginalPushesRigidBodies;
	bOriginalIgnoreForces = bOriginals.bOriginalIgnoreForces;
}

void ASafeSpawnRepInfo::SetGhostEffect(AUTCharacter* P, bool bTurnOn)
{
	if (P != NULL)
	{
		USafeSpawn::SetGhostEffectFor(P, bTurnOn);
	}
}

void ASafeSpawnRepInfo::SetThirdPerson(APlayerController* PC, bool bEnable)
{
	if (PC == NULL)
		return;

	USafeSpawn::SetThirdPersonFor(PC, bEnable, bOriginalBehindView);
}