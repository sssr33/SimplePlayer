#include "ILockLS.h"
#include "ThreadLockStackLS.h"

namespace thread {
	ILock::ILock() {
		auto lockStack = ThreadLockStackSingleton::GetInstance();
		lockStack->Add(this);
	}

	ILock::~ILock() {
		auto lockStack = ThreadLockStackSingleton::GetInstance();
		lockStack->Remove(this);
	}
}