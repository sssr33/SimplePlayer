#include "StdMutexLockLS.h"

namespace thread {
	StdMutexLock::StdMutexLock(std::mutex &mtx)
		: mtx(mtx), lock(mtx)
	{
	}

	StdMutexLock::StdMutexLock(std::unique_ptr<std::mutex> &mtx)
		: mtx(*mtx), lock(*mtx)
	{
	}

	StdMutexLock::StdMutexLock(std::shared_ptr<std::mutex> &mtx)
		: mtx(*mtx), lock(*mtx)
	{
	}

	StdMutexLock::~StdMutexLock() {
	}

	void StdMutexLock::Lock() {
		this->mtx.lock();
	}

	void StdMutexLock::Unlock() {
		this->mtx.unlock();
	}
}