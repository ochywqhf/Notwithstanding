#include "Interaction/NwsWeapon.h"
#include "Player/NwsCharacter.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

ANwsWeapon::ANwsWeapon()
	: Socket(CreateDefaultSubobject<USceneComponent>(TEXT("Socket")))
	, Mesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh")))
	, ArmMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh")))
{
	RootComponent = Socket;

	Mesh->SetupAttachment(RootComponent);
	Mesh->SetSimulatePhysics(true);
	Mesh->CanCharacterStepUpOn = ECB_No;
	Mesh->SetCollisionProfileName(TEXT("WeaponDropped"));
	Mesh->SetIsReplicated(true);

	ArmMesh->SetupAttachment(RootComponent);
	ArmMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ArmMesh->CastShadow = false;
	ArmMesh->bLightAttachmentsAsGroup = true;
	ArmMesh->SetIsReplicated(true);
	ArmMesh->SetOnlyOwnerSee(true);
}

void ANwsWeapon::ClientTogglePerspective_Implementation(bool bInUsingFirstPersonView)
{
	Mesh->SetOwnerNoSee(bInUsingFirstPersonView);
	ArmMesh->SetVisibility(bInUsingFirstPersonView);
}

void ANwsWeapon::SetOwner(AActor* NewOwner)
{
	if (auto* Interactor = Cast<ANwsCharacter>(NewOwner ? NewOwner : GetOwner()))
	{
		Super::SetOwner(NewOwner);

		bCanBeInteracted = !NewOwner;

		Mesh->SetSimulatePhysics(!NewOwner);
		Mesh->SetCollisionProfileName(NewOwner ? TEXT("WeaponPicked") : TEXT("WeaponDropped"));
		Mesh->CastShadow = !NewOwner;

		ArmMesh->SetSkeletalMesh(NewOwner ? Mesh->SkeletalMesh : nullptr);

		if (NewOwner)
		{
			Mesh->AttachToComponent(Interactor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
			ArmMesh->AttachToComponent(Interactor->GetArmMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
		}
		else
		{
			Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			ArmMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ANwsWeapon::Interact(ANwsCharacter* Interactor)
{
	if (Interactor)
		Interactor->ServerPickUpWeapon(this);
}

void ANwsWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANwsWeapon, AmmoNum);
}
