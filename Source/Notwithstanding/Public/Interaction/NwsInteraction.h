#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NwsInteraction.generated.h"

UCLASS()
class NOTWITHSTANDING_API ANwsInteraction : public AActor
{
	GENERATED_BODY()
	
public:	
	ANwsInteraction();

	bool CanBeInteracted() const { return bCanBeInteracted; }
	float GetInteractionTime() const { return InteractionTime; }
	const FString& GetDescription() const { return Description; }
	const class UTexture2D* GetIcon() const { return Icon; }

	virtual void Interact(class ANwsCharacter* Interactor) {}

	virtual void BeginHover(class ANwsCharacter* Interactor) {}
	virtual void EndHover() {}

	virtual void BeginInteracting(class ANwsCharacter* Interactor) {}
	virtual void EndInteracting() {}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, EditDefaultsOnly)
	bool bCanBeInteracted = true;

	UPROPERTY(EditDefaultsOnly)
	float InteractionTime;

	UPROPERTY(EditDefaultsOnly)
	FString Description;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* Icon;
};
