// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Describes interaction logic for objects, that will use it
 */
class NINTERACTION_API IInteractionInterface
{
	GENERATED_BODY()

public:
#pragma region MainMethods
	/**
	 * System will call this procedure before interaction, must return true, if this object can interact with given instigator
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	bool CanInteractWith(const AActor* Instigator) const;

	/**
	 * Starts interaction for this object with given instigator,
	 * when this procedure is called it means that interaction timer in instigator's component started his interaction timer
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void StartInteraction(AActor* Instigator);

	/**
	 * Performs interaction logic of object, will be called between start and end interaction, if it was successful
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact();
	
	/**
	 * Ends interaction for this object.
	 * by default, it will be called after interact, or if interaction was cancelled
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void EndInteraction(const bool bWasBroken);
	
#pragma endregion Main Methods

#pragma region VisualMethods
	/**
	 * Being called, when interaction component gives focus on this interactable
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnFocus(AActor* Instigator);

	/**
	 * Being called, when interaction component loses focus from this interactable
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnLostFocus();

	/**
	 * Return interactable additional data.
	 * Usually, it should be visual information, but you can put here anything you want
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	const UObject* GetInteractableData();
#pragma endregion VisualMethods
	
#pragma region GetterMethods
	/** Time user need to interact with this object, delay between start and end interaction.
	 * If it is zero, end interaction will be called after start without any delay
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	float GetInteractionTime() const;
#pragma endregion GetterMethods
};
