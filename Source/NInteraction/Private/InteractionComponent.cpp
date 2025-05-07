// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Interaction is enabled at begin play by default. I don't know, it is good, or not, but you can change it in future for your needs.
	EnableInteraction();
}

void UInteractionComponent::EnableInteraction()
{
	if (IsInteractionEnabled()) return;

	GetWorld()->GetTimerManager().SetTimer(FindTimer, this, &UInteractionComponent::InteractionTick, TickTime, true);
}

void UInteractionComponent::DisableInteraction()
{
	if (!IsInteractionEnabled()) return;

	GetWorld()->GetTimerManager().ClearTimer(FindTimer);
}

bool UInteractionComponent::IsInteractionEnabled() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(FindTimer);
}

void UInteractionComponent::StartInteraction()
{
	if (!IsInteractableValid()) return;
	if (IsInteracting()) BreakInteraction();
	
	IInteractionInterface::Execute_StartInteraction(CurrentInterface.GetObject(), GetOwner());

	const auto InteractionTime = IInteractionInterface::Execute_GetInteractionTime(CurrentInterface.GetObject());
	if (InteractionTime <= 0.0f)
	{
		PerformInteraction();
	}

	else
	{
		GetWorld()->GetTimerManager().SetTimer(InteractionTimer, this, &ThisClass::PerformInteraction, InteractionTime);
	}
}

void UInteractionComponent::PerformInteraction() const
{
	if (!IsInteractableValid()) return;

	IInteractionInterface::Execute_Interact(CurrentInterface.GetObject());
	IInteractionInterface::Execute_EndInteraction(CurrentInterface.GetObject(), false);
}


void UInteractionComponent::BreakInteraction()
{
	if (!IsInteractableValid()) return;
	if (!IsInteracting()) return;

	GetWorld()->GetTimerManager().ClearTimer(InteractionTimer);
	IInteractionInterface::Execute_EndInteraction(CurrentInterface.GetObject(), true);
}

bool UInteractionComponent::IsInteracting() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(InteractionTimer);
}

void UInteractionComponent::InteractionTick()
{
	const auto LocalTracePosition = GetOwner()->GetActorLocation();
	const auto World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);

	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingle(World, LocalTracePosition, LocalTracePosition,
		TraceRadius, UEngineTypes::ConvertToTraceType(TraceCollisionChannel), false, TArray<AActor*>(), EDrawDebugTrace::None, Hit, true);

#if WITH_EDITOR
	if (bDebugMode)
	{
		UKismetSystemLibrary::DrawDebugSphere(World, LocalTracePosition, TraceRadius, 12, Hit.bBlockingHit ? FoundColor : DefaultColor, DebugTime);
	}
#endif

	if (!Hit.bBlockingHit)
	{
		if (IsInteractableValid())
		{
			IInteractionInterface::Execute_OnLostFocus(CurrentInterface.GetObject());
			if (IsInteracting()) BreakInteraction();
			
			CurrentInterface.SetObject(nullptr);
		}

		return;
	}

	if (!IsValid(Hit.GetActor())) return;
	if (!Hit.GetActor()->Implements<UInteractionInterface>()) return;
	if (Hit.GetActor() == CurrentInterface.GetObject()) return;
	
	CurrentInterface.SetObject(Hit.GetActor());
	IInteractionInterface::Execute_OnFocus(CurrentInterface.GetObject(), GetOwner());
	OnNewInteractableFound(Hit.GetActor());
}

void UInteractionComponent::OnNewInteractableFound(AActor* Interactable)
{
	K2_OnNewInteractableFound(Interactable);
}

bool UInteractionComponent::IsInteractableValid() const
{
	return IsValid(CurrentInterface.GetObject());
}
