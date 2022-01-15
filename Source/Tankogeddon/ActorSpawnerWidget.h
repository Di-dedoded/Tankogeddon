#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorSpawnerWidget.generated.h"

class UTextBlock;

UCLASS()
class TANKOGEDDON_API UActorSpawnerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnerClass;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DraggedText;

	UPROPERTY(EditAnywhere)
	FString DraggedName;

	UPROPERTY()
	AActor* SpawnedActor = nullptr;

	UPROPERTY()
	APlayerController* PlayerController;

	void OnMouseButtonUp();

};
