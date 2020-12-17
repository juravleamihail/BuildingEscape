// Copyright Mihail Juravlea 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Delegates/Delegate.h"
#include "Components/PrimitiveComponent.h"
#include <Runtime\Engine\Classes\Sound\SoundCue.h>
#include "DoorRotation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorRotation : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UDoorRotation();
	void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onClose;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float triggerMass = 30.0f;

	UPROPERTY(EditAnywhere)
	USoundCue* SoundCue = nullptr;

	AActor* owner = nullptr;
	bool canPlay = true;

	float GetTotalMassOfActorsOnPlate();
};
