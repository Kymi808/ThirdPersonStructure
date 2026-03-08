// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryTooltip.h"

#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->Itemtype) {
	case EItemType::Armor:
		ItemType->SetText(FText::FromString("Armor"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Weapon"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Shield:
		ItemType->SetText(FText::FromString("Shield"));
		break;
	case EItemType::Spell:
		ItemType->SetText(FText::FromString("Spell"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		ItemType->SetText(FText::FromString("Quest"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Misc:
		ItemType->SetText(FText::FromString("Misc"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:;
	}
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	MaxStackSizeText->SetText(FText::FromString("Max Stack Size:"));
	// SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.SellValue));
	const FString WeightInfo =
		{"Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
	
	StackWeightValue->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
