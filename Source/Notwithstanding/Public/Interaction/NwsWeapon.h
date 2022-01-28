#pragma once

#include "CoreMinimal.h"
#include "Interaction/NwsInteraction.h"
#include "NwsWeapon.generated.h"

UENUM(BlueprintType)
enum class ENwsWeaponType : uint8
{
	Primary,
	Secondary,
	Melee,
	None
};

UCLASS()
class NOTWITHSTANDING_API ANwsWeapon : public ANwsInteraction
{
	GENERATED_BODY()
	
public:
	ANwsWeapon();

	ENwsWeaponType GetWeaponType() const { return WeaponType; }
	int32 GetAmmoNum() const { return AmmoNum; }

	UFUNCTION(Client, Reliable)
	void ClientTogglePerspective(bool bInUsingFirstPersonView);

	virtual void SetOwner(AActor* NewOwner) override;
	virtual void Interact(class ANwsCharacter* Interactor) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	class USceneComponent* Socket;

	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditDefaultsOnly)
	ENwsWeaponType WeaponType = ENwsWeaponType::None;

	UPROPERTY(Replicated, EditDefaultsOnly)
	int32 AmmoNum;
};
