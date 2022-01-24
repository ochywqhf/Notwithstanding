#include "Player/NwsCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ANwsCharacter::ANwsCharacter()
	: SpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")))
	, Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")))
	, ArmMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh")))
{
	bAlwaysRelevant = true;
	NetUpdateFrequency = 60.f;
	NetPriority = 4.f;
	SetCanBeDamaged(false);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetOwnerNoSee(true);

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(20.f, 0.f, 70.f));
	SpringArm->TargetArmLength = 0.f;
	SpringArm->ProbeSize = 5.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 20.f;

	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeScale3D(FVector(0.25f));

	ArmMesh->SetupAttachment(Camera);
	ArmMesh->SetRelativeLocation(FVector(4.f, 0.f, -165.5f));
	ArmMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ArmMesh->bSelfShadowOnly = true;
	ArmMesh->bLightAttachmentsAsGroup = true;
	ArmMesh->SetOnlyOwnerSee(true);

	GetCharacterMovement()->MaxWalkSpeed = 360.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 170.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ANwsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANwsCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANwsCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANwsCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ANwsCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("TogglePerspective"), IE_Pressed, this, &ANwsCharacter::TogglePerspective);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ANwsCharacter::Jumping);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ANwsCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ANwsCharacter::Crouching);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ANwsCharacter::Running);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ANwsCharacter::Running);
}

void ANwsCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ANwsCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ANwsCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ANwsCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ANwsCharacter::TogglePerspective()
{
	bUseFirstPersonView = !bUseFirstPersonView;

	GetMesh()->SetOwnerNoSee(bUseFirstPersonView);
	ArmMesh->SetVisibility(bUseFirstPersonView);

	SpringArm->TargetArmLength = bUseFirstPersonView ? 0.f : 200.f;
	SpringArm->SetRelativeLocation(FVector(20.f, (bUseFirstPersonView ? 0.f : 35.f), 70.f));
}

void ANwsCharacter::Jumping()
{
	bCrouching ? UnCrouch() : Jump();
	bCrouching = false;
}

void ANwsCharacter::Crouching()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	bCrouching = !bCrouching;
	bCrouching ? Crouch() : UnCrouch();

	if (bRunning)
		CheckRunning(!bCrouching);
}

void ANwsCharacter::Running()
{
	bRunning = !bRunning;

	if (bRunning && bCrouching)
	{
		bCrouching = false;
		UnCrouch();
	}

	CheckRunning(bRunning);
}

void ANwsCharacter::CheckRunning(bool bInRunning)
{
	uint8 RunningShake = static_cast<uint8>(ENwsCameraShakeType::Run);
	GetCharacterMovement()->MaxWalkSpeed = bInRunning ? 540.f : 360.f;

	if (UGameplayStatics::GetPlayerCameraManager(this, 0) && CameraShakeClasses[RunningShake])
	{
		auto* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		bInRunning ? CameraManager->StartCameraShake(CameraShakeClasses[RunningShake]) : CameraManager->StopAllCameraShakes();
	}

	ServerRunning(bInRunning);
}

void ANwsCharacter::ServerRunning_Implementation(bool bInRunning)
{
	GetCharacterMovement()->MaxWalkSpeed = bInRunning ? 540.f : 360.f;
}
