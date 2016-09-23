#pragma once
#include "ILockListItemLS.h"

namespace thread {
	class ILock : public ILockListItem {
	public:
		ILock();
		virtual ~ILock();
	};
}