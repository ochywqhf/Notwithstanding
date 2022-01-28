#include "Interaction/NwsInteraction.h"

#include "Net/UnrealNetwork.h"

ANwsInteraction::ANwsInteraction()
{
	bAlwaysRelevant = true;
	SetReplicateMovement(true);
	bReplicates = true;
}

void ANwsInteraction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANwsInteraction, bCanBeInteracted);
}
