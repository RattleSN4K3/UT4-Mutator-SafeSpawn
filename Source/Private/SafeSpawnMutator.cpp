#include "SafeSpawn.h"

#include "UTWeapon.h"
#include "UTWeap_Enforcer.h"
#include "UTPickup.h"
#include "UTPickupHealth.h"
#include "UTPickupAmmo.h"
//#include "UTTimedPowerup.h"
//#include "UTJumpBoots.h"

#include "SafeSpawnMutator.h"

ASafeSpawnMutator::ASafeSpawnMutator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("Mutator_SafeSpawn", "Display Name", "Safe Spawn");

	NoSound = ConstructObject<USoundBase>(USoundCue::StaticClass());

	DamageHelperClass = ASafeSpawnInventory::StaticClass();

	// default config
	GhostProtectionTime = 6.0f;
	InitialFireDelay = 0.0f;
}

//**********************************************************************************
// Inherited funtions
//**********************************************************************************

void ASafeSpawnMutator::BeginPlay()
{
	Super::BeginPlay();

	// early out
	AGameMode* GM = GetWorld()->GetAuthGameMode();
	if (GM == nullptr)
		return;

	TSubclassOf<class AUTCharacter> DefaultPawnClass(*GM->DefaultPawnClass);

	// safe check before adding game rules etc.
	if (GhostProtectionTime > 0 && DefaultPawnClass != NULL)
	{
		//RestoreSpawnSound = DefaultPawnClass.GetDefaultObject()->SpawnSound;
		RestoreTeleportEffect = DefaultPawnClass.GetDefaultObject()->TeleportEffect[0];

		// Hush
		//InventoryWarningSound = DefaultPawnClass.GetDefaultObject()->SpawnSound;

		// Zirp (Translocate)
		//InventoryTimeoutSound = DefaultPawnClass.GetDefaultObject()->TeleportSound;
	}
}

void ASafeSpawnMutator::PostPlayerInit_Implementation(AController* C)
{
	Super::PostPlayerInit_Implementation(C);

	ConditionallyAddPRIFor(Cast<APlayerController>(C));
	ConditionallyAddLinkedRIFor(C);
}

void ASafeSpawnMutator::NotifyLogout_Implementation(AController* C)
{
	RemovePRIFrom(Cast<APlayerController>(C));
	RemoveLinkedRIFrom(C);

	Super::NotifyLogout_Implementation(C);
}

bool ASafeSpawnMutator::CheckRelevance_Implementation(AActor* Other)
{
	// remove the initial Enforcer reload time
	//if (Cast<AUTWeap_Enforcer>(Other) != NULL)
	//{
	//	Cast<AUTWeap_Enforcer>(Other)->bLoaded = true;
	//}

	AUTCharacter* Char = Cast<AUTCharacter>(Other);
	if (Char != NULL)
	{
		// Remove old spawn sounds
		//Char->SpawnSound = NoSound;

		// Remove spawn effect (for all team; no need to set others as the code checks only for the first)
		Char->TeleportEffect[0] = NULL;
	}

	return Super::CheckRelevance_Implementation(Other);
}

// TODO: Add Vehicle support
//void ASafeSpawnMutator::DriverEnteredVehicle_Implementation(AVehicle* V, APawn* P)
//{
//	Super::DriverEnteredVehicle_Implementation(V, P);
//
//	AUTInventory* inv = NULL;
//	if (Cast<ACharacter>(P) != NULL && HasInventory(P, inv))
//	{
//		//@CHECK: vehicle collsion; if the player can be killed by entering a vehicle while being ghost protected
//		ProtectPlayer(Cast<ACharacter>(P), false);
//	}
//}

void ASafeSpawnMutator::ModifyPlayer_Implementation(APawn* Other, bool bIsNewSpawn)
{
	Super::ModifyPlayer_Implementation(Other, bIsNewSpawn);

	// prevent JumpBoots from callign ModifyPlayer again (and thus protecting the player again)
	if (!bIsNewSpawn)
		return;

	// TODO: add Ghost protection ignore when bots actually ignore Spawn protected players
	//// adjust spawn times so bots will ignore the bots on spawn
	////
	//// as bots are generally ignoring bots with the default spawn protection,
	//// this won't work for the ghost protection as we disbaling it
	////
	//// WorldInfo.TimeSeconds - Pawn.SpawnTime < UTDeathMatch(WorldInfo.Game).SpawnProtectionTime)
	//if (UTGame(WorldInfo.Game) != none)
	//{
	//	offset = UTGame(WorldInfo.Game).SpawnProtectionTime;
	//}
	//Other.SpawnTime += (GhostProtectionTime - offset);


	AUTCharacter* Char = Cast<AUTCharacter>(Other);
	if (Char != NULL)
	{
		// restore spawn effect
		Char->TeleportEffect[0] = RestoreTeleportEffect;

		ProtectPlayer(Char, true);
	}
}

//**********************************************************************************
// Inherited GameRules funtions
//**********************************************************************************

bool ASafeSpawnMutator::ModifyDamage_Implementation(UPARAM(ref) int32& Damage, UPARAM(ref) FVector& Momentum, APawn* Injured, AController* InstigatedBy, const FHitResult& HitInfo, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	Super::ModifyDamage_Implementation(Damage, Momentum, Injured, InstigatedBy, HitInfo, DamageCauser, DamageType);

	// check if the player helds the Damage helper inventory
	// which will block the damage

	AUTCharacter* Char = Cast<AUTCharacter>(Injured);
	if (Char != NULL && DamageType != NULL && !DamageType.GetDefaultObject()->bCausedByWorld && HasInventory(Char))
	{
		Momentum = FVector(0.0f);
		Damage = 0;
	}

	// TODO FIX: remove return value.
	return true;
}

bool ASafeSpawnMutator::OverridePickupQuery_Implementation(APawn* Other, TSubclassOf<AUTInventory> ItemClass, AActor* Pickup, bool& bAllowPickup)
{
	AUTInventory* inv = NULL;
	AUTCharacter* Char = Cast<AUTCharacter>(Other);
 	if (Char != NULL && HasInventory(Char, inv))
	{
		// don't disable ghost protection if player attempted to pickup
		// Health packs if they aren't adding health (if not SuperHealth)

		// TODO: Wait for engine fix as AUTPickupHealth is not exported properly
		if (Cast<AUTPickupHealth>(Pickup) != NULL && (Cast<AUTPickupHealth>(Pickup)->bSuperHeal == false || Char->Health != Char->HealthMax))
		{
			Super::OverridePickupQuery_Implementation(Other, ItemClass, Pickup, bAllowPickup);
			bAllowPickup = false;
			return true;
		}

		// also ammo packs shouldn't remove protection
		if (Cast<AUTPickupAmmo>(Pickup) == NULL)
		{
			// destroy inventory if there's still one
			if (!inv->IsPendingKillPending())
			{
				if (Cast<AUTTimedPowerup>(Pickup) != NULL)
					Cast<AUTTimedPowerup>(Pickup)->TimeExpired(); // call TimeExpired to trigger PowerupOver sound
				else
					inv->Destroy();
			}
		}
	}

	return Super::OverridePickupQuery_Implementation(Other, ItemClass, Pickup, bAllowPickup);
}

//**********************************************************************************
// Delegate Callbacks
//**********************************************************************************

void ASafeSpawnMutator::OnUnProtectFire(APlayerController* PC, ASafeSpawnRepInfo* ClientRI)
{
	ProtectPlayer(PC != NULL ? Cast<AUTCharacter>(PC->GetCharacter()) : NULL, false, ClientRI);
}

// TODO: Fix include to use AUTCharacter instead of ACharacter
void ASafeSpawnMutator::OnUnProtectPickup(ACharacter* P)
{
	AUTCharacter* Char = Cast<AUTCharacter>(P);
	if (Char != NULL)
	{
		ProtectPlayer(Char, false);
	}
}

//**********************************************************************************
// Private funtions
//**********************************************************************************

void ASafeSpawnMutator::ConditionallyAddPRIFor(APlayerController* PC)
{
	if (PC == NULL)
		return;

	ASafeSpawnRepInfo* ClientRI = NULL;
	if (!GetClientRI(PC, ClientRI))
	{
		FActorSpawnParameters Params;
		Params.bNoCollisionFail = true;
		Params.Owner = PC;
		ClientRI = GetWorld()->SpawnActor<ASafeSpawnRepInfo>(ASafeSpawnRepInfo::StaticClass(), Params);
		if (ClientRI != NULL)
		{
			ClientRI->InitialSetup(PC, FUnProtectFireDelegate::CreateUObject(this, &ASafeSpawnMutator::OnUnProtectFire), InitialFireDelay);
		}
	}
}

void ASafeSpawnMutator::RemovePRIFrom(APlayerController* PC)
{
	if (PC == NULL)
		return;

	ASafeSpawnRepInfo* ClientRI = NULL;
	if (GetClientRI(PC, ClientRI))
	{
		ClientRI->Destroy();
	}
}

void ASafeSpawnMutator::ConditionallyAddLinkedRIFor(AController* C)
{
	if (C == NULL)
		return;

	ASafeSpawnLink* LinkedRI = NULL;
	if (!GetLinkedRI(C, LinkedRI))
	{
		FActorSpawnParameters Params;
		Params.bNoCollisionFail = true;
		Params.Owner = C;
		LinkedRI = GetWorld()->SpawnActor<ASafeSpawnLink>(ASafeSpawnLink::StaticClass(), Params);
	}
}

void ASafeSpawnMutator::RemoveLinkedRIFrom(AController* C)
{
	if (C == NULL)
		return;

	ASafeSpawnLink* LinkedRI = NULL;
	if (GetLinkedRI(C, LinkedRI))
	{
		LinkedRI->Destroy();
	}
}

void ASafeSpawnMutator::ProtectPlayer(AUTCharacter* Other, bool bProtect, ASafeSpawnRepInfo*& ClientRI)
{
	APlayerController* PC = NULL;
	if (GetPC(Other, PC) && ClientRI == NULL&& !GetClientRI(PC, ClientRI))
	{
#if UE_BUILD_DEBUG
		PC->ClientMessage(TEXT("No ClientRI"));
#endif
		// Unable to find ClientRI for connected client, abort
		return;
	}

	AUTInventory* inv = NULL;
	ASafeSpawnLink* LinkedRI = NULL;
	if (!bProtect)
	{
		if (Other != NULL)
		{
			//// revert skin
			//Other.SetSkin(none);

			if (Other->Health > 0) // if not suicided
			{
				//Other->PlayTeleportEffect(false, true);
			}

			// destroy inventory if there's still one
			if (HasInventory(Other, inv) && !inv->IsPendingKillPending())
			{
				inv->Destroy();
			}

			// remove protection from any Pawn
			if (GetLinkedRI(GetController(Other), LinkedRI))
			{
				LinkedRI->NotfiyRemove();
			}

			// check if we should kill someone or die if we spawn in a colliding object
			USafeSpawn::CheckSpawnKill(Other);
		}

		if (ClientRI != NULL)
		{
			ClientRI->NotifyActive();
		}
	}
	else
	{
		// remove original spawn protection
		Other->DeactivateSpawnProtection();

		// Restore old spawn sounds (if another mods are calling it during the game)
		//Other->SpawnSound = RestoreSpawnSound;

		// spawn inventory for Sound, Pickup check and helping messages
		GiveInventory(Other);

		// TODO: Add support for lockers.
		//if (HasInventory(Other, inv) && Cast<ASafeSpawnInventory>(inv) != none &&
		//	GetWorld()->GetAuthGameMode<AUTGameMode>() != NULL && GetWorld()->GetAuthGameMode<AUTGameMode>()->bStartWithLockerWeaps)
		//{
		//	Cast<ASafeSpawnInventory>(inv)->bWaitForLanded = true;
		//}

		// link pawn so any other Pawn becomes a full ghost
		if (GetLinkedRI(GetController(Other), LinkedRI))
		{
			LinkedRI->NotfiyNewPawn(Other);
		}

		if (ClientRI != NULL)
		{
			// notify rep
			//UE_LOG(LogTemp, Log, TEXT("Call NotifyRespawned for %s"), *ClientRI->PlayerOwner->GetName());
			ClientRI->NotifyRespawned(Other);
		}
	}
}

bool ASafeSpawnMutator::GiveInventory(AUTCharacter* Other)
{
	if (Other == NULL || HasInventory(Other))
	{
		return false;
	}

	ASafeSpawnInventory* inv = GetWorld()->SpawnActor<ASafeSpawnInventory>(DamageHelperClass);
	if (inv != NULL)
	{
		//inv->PowerupOverSound = InventoryTimeoutSound;

		// Set the time first
		inv->TimeRemaining = GhostProtectionTime;

		// Add the inventory (which also updates TimeRemaining for clients)
		Other->AddInventory(inv, false);

		// setup (which sets time and replicates sound cue)		
		inv->SetupInventory(Other, InventoryWarningSound, GhostProtectionTime, FUnProtectPickupDelegate::CreateUObject(this, &ASafeSpawnMutator::OnUnProtectPickup));

		return true;
	}

	return false;
}

//'''''''''''''''''''''''''
// Helper
//'''''''''''''''''''''''''

bool ASafeSpawnMutator::HasInventory(AUTCharacter* Other)
{
	AUTInventory* inv;
	return HasInventory(Other, inv);
}

bool ASafeSpawnMutator::HasInventory(AUTCharacter* Other, AUTInventory*& inv)
{
	inv = Other->FindInventoryType<AUTInventory>(DamageHelperClass, true);
	return (inv != NULL);
}

bool ASafeSpawnMutator::GetPC(ACharacter* P, APlayerController*& PC)
{
	if (P == NULL)
		return false;

	PC = Cast<APlayerController>(P->Controller);
	// TODO: Add Vehicle support
	//if (PC == NULL && P->DrivenVehicle != NULL)
	//{
	//	PC = UTPlayerController(P->DrivenVehicle->Controller);
	//}

	return PC != NULL;
}

AController* ASafeSpawnMutator::GetController(ACharacter* P)
{
	if (P == NULL)
		return NULL;

	AController* C = P->Controller;
	// TODO: Add Vehicle support
	//if (C == NULL && P->DrivenVehicle != NULL)
	//{
	//	C = P->DrivenVehicle->Controller;
	//}

	return C;
}

bool ASafeSpawnMutator::GetClientRI(APlayerController* PC, ASafeSpawnRepInfo*& out_ClientRI)
{
	for (auto Child : PC->Children)
	{
		ASafeSpawnRepInfo* Rep = Cast<ASafeSpawnRepInfo>(Child);
		if (Rep != NULL)
		{
			out_ClientRI = Rep;
			break;
		}
	}

	return out_ClientRI != NULL;
}

bool ASafeSpawnMutator::GetLinkedRI(AController* C, ASafeSpawnLink*& out_LinkedRI)
{
	if (C != NULL)
	{
		for (auto Child : C->Children)
		{
			ASafeSpawnLink* Link = Cast<ASafeSpawnLink>(Child);
			if (Link != NULL)
			{
				out_LinkedRI = Link;
				break;
			}
		}
	}

	return out_LinkedRI != NULL;
}
