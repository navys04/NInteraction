// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractionInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewInteractableFoundDelegate, const AActor*, Interactable);

/**
 * This component should be added to player's character in game. Controls all interaction related logic.
 * Find and interact with this system objects through this component, please ^_^
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NINTERACTION_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

#pragma region StateMethods
	/**
	 * Enables interaction find logic
	 */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void EnableInteraction();

	/**
	 * Disables interaction find logic
	 */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void DisableInteraction();

	/**
	 * Will return true if interaction is enabled at this frame
	 */
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool IsInteractionEnabled() const;
#pragma endregion StateMethods

#pragma region EventMethods
	/**
	 * Starts interaction with current object if it exists
	 */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void StartInteraction();

	/**
	 * Breaks interaction, if it was not completed
	 */
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void BreakInteraction();

	/**
	 * Return true if we are interacting with object at this frame
	 */
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool IsInteracting() const;
#pragma endregion EventMethods

#pragma region Delegates
	/**
	 * Calls when this component founds and confirms new interactable
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnNewInteractableFoundDelegate OnNewInteractableFoundDelegate;
#pragma endregion Delegates

protected:
	virtual void BeginPlay() override;

#pragma region NativeMethods
	/**
	 * Will be called when system is found and already focused on new object
	 * by default only calls blueprint version, but you can override it and write your own handler
	 */
	virtual void OnNewInteractableFound(AActor* Interactable);
#pragma endregion NativeMethods

private:

#pragma region Settings
	/**
	 * Component will try to find new interaction objects with this frequency
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings", meta = (AllowPrivateAccess = "true"))
	float TickTime;

	/**
	 * Radius of scan-sphere
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings", meta = (AllowPrivateAccess = "true"))
	float TraceRadius;

	/**
	 * Collision channel for sphere trace
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;

#if WITH_EDITORONLY_DATA
	/**
	 * Works only in editor, you can't use this functionality in shipping builds
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings|Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebugMode;

	/**
	 * Default color of debug sphere
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings|Debug", meta = (AllowPrivateAccess = "true"))
	FLinearColor DefaultColor;

	/**
	 * Found color of debug sphere
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings|Debug", meta = (AllowPrivateAccess = "true"))
	FLinearColor FoundColor;

	/**
	 * Sphere will hide after this time
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction Component|Settings|Debug", meta = (AllowPrivateAccess = "true"))
	float DebugTime;
#endif WITH_EDITORONLY_DATA
#pragma endregion Settings;
	
#pragma region InternalVariables
	/**
	 * This handle is used for interaction tick
	 */
	UPROPERTY()
	FTimerHandle FindTimer;

	/**
	 * This handle is used for delay between start and end interaction
	 */
	UPROPERTY()
	FTimerHandle InteractionTimer;

	/**
	 * Current interaction interface,
	 * his validity does not guarantee that we are interacting with it at this frame
	 */
	UPROPERTY()
	TScriptInterface<IInteractionInterface> CurrentInterface;
#pragma endregion InternalVariables

#pragma region InternalMethods
	/**
	 * This procedure being called every tick time, that user set,
	 * attempts to find new interaction object, or update information about current
	 */
	void InteractionTick();

	/**
	 * This procedure will perform interaction on current object if it exists, and it is realizable
	 */
	void PerformInteraction() const;
#pragma endregion InternalMethods

#pragma region HelperMethods
	/**
	 *  This is double check for both object pointer, and his internal TScriptInterface
	 *  you can write this everywhere by your own, but I think this will help you ^_^
	 */
	bool IsInteractableValid() const;
#pragma endregion HelperMethods
};
