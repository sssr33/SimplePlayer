#include "LockStack.h"
#include "LockStack\ThreadLockStackLS.h"
#include "libhelpers\HSystem.h"

namespace thread {
	LockStack::ScopedUnlock::ScopedUnlock() {
		auto lockStack = ThreadLockStackSingleton::GetInstance();
		this->lockRange = lockStack->UnlockRange();
	}

	LockStack::ScopedUnlock::~ScopedUnlock() {
		auto lockStack = ThreadLockStackSingleton::GetInstance();
		lockStack->Lock(this->lockRange);
	}
}