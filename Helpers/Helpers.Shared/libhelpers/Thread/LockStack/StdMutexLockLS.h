#pragma once
#include "ILockLS.h"

#include <mutex>
#include <memory>

namespace thread {
	class StdMutexLock : protected ILock {
	public:
		StdMutexLock(std::mutex &mtx);
		StdMutexLock(std::unique_ptr<std::mutex> &mtx);
		StdMutexLock(std::shared_ptr<std::mutex> &mtx);
		virtual ~StdMutexLock();

	protected:
		void Lock() override;
		void Unlock() override;

	private:
		std::mutex &mtx;
		std::unique_lock<std::mutex> lock;
	};
}