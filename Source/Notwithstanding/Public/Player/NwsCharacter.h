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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
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
};
