// Copyright Mihail Juravlea 2020


#include "DoorRotation.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UDoorRotation::UDoorRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UDoorRotation::OpenDoor()
{
	AActor* owner = GetOwner();

	FRotator const Rotator = FRotator(0, 90, 0);

	owner->SetActorRotation(Rotator);
}

// Called when the game starts
void UDoorRotation::BeginPlay()
{
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(pressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
}

