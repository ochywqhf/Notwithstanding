#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NwsCharacter.generated.h"

UENUM(BlueprintType)
enum class ENwsCameraShakeType : uint8
{
	Run
};

UCLASS()
class NOTWITHSTANDING_API ANwsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANwsCharacter();

	class USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; }

	UFUNCTION(Server, Reliable)
	void ServerPickUpWeapon(class ANwsWeapon* Weapon);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Client, Reliable)
	void ClientInit();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void TogglePerspective();
	void Jumping();
	void Crouching();
	void Running();
	void CheckRunning(bool bInRunning);

	UFUNCTION(Server, Reliable)
	void ServerRunning(bool bInRunning);

	void Interacting();
	void StopInteracting();

	void TryToInteract();
	void CheckInteracting();

	void Interact();
	void BeginInteracting();
	void EndInteracting();

	UFUNCTION(Server, Reliable)
	void ServerInteracting(class ANwsInteraction* Target, ANwsCharacter* Interactor, bool bBeginning);

	UFUNCTION(Client, Reliable)
	void ClientStopInteracting();

	UFUNCTION(Server, Reliable)
	void ServerDropOffWeapon();

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class UCameraShakeBase>> CameraShakeClasses;

	bool bUseFirstPersonView = true;
	bool bCrouching = false;
	bool bRunning = false;
	bool bTryToInteract = false;
	bool bInteracting = false;

	FTimerHandle CheckInteractionTimerHandle;
	FTimerHandle InteractionTimerHandle;

	UPROPERTY(Replicated)
	class ANwsInteraction* Interaction;

	UPROPERTY(Replicated)
	TArray<class ANwsWeapon*> Inventory;

	UPROPERTY(Replicated)
	uint8 InventoryIndex;
};
