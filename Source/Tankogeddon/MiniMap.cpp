// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMap.h"
#include "SMiniMap.h"

void UMiniMap::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MiniMap.Reset();
}

TSharedRef<SWidget> UMiniMap::RebuildWidget()
{
	MiniMap = SNew(SMiniMap)
		.PlayerPos_UObject(this, &ThisClass::GetPlayerPos)
		.Obstacles(Obstacles)
		.PlayerImage(PlayerImage);

	return MiniMap.ToSharedRef();
}

FVector2D UMiniMap::GetPlayerPos() const
{
	FVector2D PlayerPos;

	PlayerPos.X = PlayerPosXDelegate.IsBound() ? PlayerPosXDelegate.Execute() : PlayerPosX;
	PlayerPos.Y = PlayerPosYDelegate.IsBound() ? PlayerPosYDelegate.Execute() : PlayerPosY;

	return PlayerPos;
}