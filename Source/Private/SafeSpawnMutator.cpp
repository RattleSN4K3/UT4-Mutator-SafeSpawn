#include "SafeSpawn.h"
#include "SafeSpawnMutator.h"

ASafeSpawnMutator::ASafeSpawnMutator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("Mutator_SafeSpawn", "Display Name", "Safe Spawn");
}

//**********************************************************************************
// Inherited funtions
//**********************************************************************************

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
	return Super::CheckRelevance_Implementation(Other);
}

void ASafeSpawnMutator::ModifyPlayer_Implementation(APawn* Other)
{
	Super::ModifyPlayer_Implementation(Other);

	AUTCharacter* Char = Cast<AUTCharacter>(Other);
	if (Char != NULL)
	{
		// restore spawn effect
		//Char.TransInEffects[0] = RestoreTransInEffect;

		ProtectPlayer(Char, true);
	}
}

//**********************************************************************************
// Inherited GameRules funtions
//**********************************************************************************

void ASafeSpawnMutator::ModifyDamage_Implementation(int32& Damage, FVector& Momentum, APawn* Injured, AController* InstigatedBy, const FHitResult& HitInfo, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
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
}

bool ASafeSpawnMutator::OverridePickupQuery_Implementation(APawn* Other, TSubclassOf<AUTInventory> ItemClass, AActor* Pickup, bool& bAllowPickup)
{
	AUTInventory* inv;
	AUTCharacter* Char = Cast<AUTCharacter>(Other);
	if (Char != NULL && HasInventory(Char, inv))
	{

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

void ASafeSpawnMutator::OnUnProtectPickup(AUTCharacter* P)
{
	ProtectPlayer(P, false);
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
			FUnProtectFireDelegate Delegate;
			//Delegate.BindSP(this, &ASafeSpawnMutator::OnUnProtectFire);

			//Delegate.BindUObject(this, &ASafeSpawnMutator::OnUnProtectFire);
			//ClientRI->InitialSetup(PC, Delegate, InitialFireDelay);

			//ClientRI->InitialSetup(PC, FUnProtectFireDelegate::CreateUObject(this, &ASafeSpawnMutator::OnUnProtectFire), InitialFireDelay);
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
	if (GetPC(Other, PC) && ClientRI == NULL && !GetClientRI(PC, ClientRI))
	{
		// Unable to find ClientRI for connected client, abort
		return;
	}
}

bool ASafeSpawnMutator::GiveInventory(AUTCharacter* Other)
{
	if (Other == NULL || HasInventory(Other))
	{
		return false;
	}

	return false;
}
