#pragma once
#include "ILockLS.h"

#include <ppl.h>
#include <memory>

namespace thread {
	class PplCsLock : protected ILock {
	public:
		PplCsLock(concurrency::critical_section &cs);
		PplCsLock(std::unique_ptr<concurrency::critical_section> &cs);
		PplCsLock(std::shared_ptr<concurrency::critical_section> &cs);
		virtual ~PplCsLock();

	protected:
		void Lock() override;
		void Unlock() override;

	private:
		concurrency::critical_section &cs;
		concurrency::critical_section::scoped_lock lock;
	};
}