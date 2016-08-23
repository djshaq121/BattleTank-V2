//Copyright Shaquille Etienne

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "Tank.h"//So we can impliment onDeath
#include "TankAIController.h"
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void  ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if(InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		//Subscribe our local methods to the tank deaths
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossesTankDeath);
	}
}

void ATankAIController::OnPossesTankDeath()
{
	if (!ensure(GetPawn())) { return; }//TODO remove if ok
	GetPawn()->DetachFromControllerPendingDestroy();
}
// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // TODO limit firing rate
	}
}
