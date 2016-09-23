#include "ILockListItemLS.h"

namespace thread {
	ILockListItem::ILockListItem() 
		: Prev(nullptr), Next(nullptr)
	{
	}

	ILockListItem::~ILockListItem() {
	}
}