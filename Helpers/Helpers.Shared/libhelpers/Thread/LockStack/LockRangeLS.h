#pragma once
#include "ILockListItemLS.h"

namespace thread {
	struct LockRange {
		// first lock on thread stack
		ILockListItem *first;
		// last lock on thread stack
		ILockListItem *last;

		LockRange();
	};
}