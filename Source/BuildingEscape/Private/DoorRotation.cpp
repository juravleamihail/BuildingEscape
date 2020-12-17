// Copyright Mihail Juravlea 2020


#include "DoorRotation.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

// Sets default values for this component's properties
UDoorRotation::UDoorRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDoorRotation::CloseDoor()
{
	if (owner == nullptr)
	{
		return;
	}
	
	owner->SetActorRotation(FRotator(0, 0, 0));
}

// Called when the game starts
void UDoorRotation::BeginPlay()
{
	owner = GetOwner();
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UAudioComponent* audioComponent = nullptr;
	if((GetTotalMassOfActorsOnPlate() > triggerMass))
	{
		canPlay = false;
		onOpen.Broadcast();

		//TODO: add sounds when you open the door and stop it after through code
		//audioComponent = UGameplayStatics::SpawnSoundAtLocation(this, SoundCue, GetOwner()->GetTargetLocation(), FRotator::ZeroRotator);
	}
	else
	{
		canPlay = true;
		onClose.Broadcast();
	}
}

float UDoorRotation::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0;

	TArray<AActor*> overlappingActors;

	if(pressurePlate == nullptr)
	{
		return 0;
	}
	
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	for (const auto& actor : overlappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *actor->GetName())
	}

	return totalMass;
}

