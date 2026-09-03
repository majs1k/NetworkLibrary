#pragma once
#include <string>

struct Icon
{
	int inventoryId_;
	int iconId_;

	void Initialize(int inventoryId, int iconId)
	{
		inventoryId_ = inventoryId;
		iconId_ = iconId;
	}
};
