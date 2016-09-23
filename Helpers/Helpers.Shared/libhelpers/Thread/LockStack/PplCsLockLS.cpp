#include "PplCsLockLS.h"

namespace thread {
	PplCsLock::PplCsLock(concurrency::critical_section &cs) 
		: cs(cs), lock(cs)
	{
	}

	PplCsLock::PplCsLock(std::unique_ptr<concurrency::critical_section> &cs)
		: cs(*cs), lock(*cs)
	{
	}

	PplCsLock::PplCsLock(std::shared_ptr<concurrency::critical_section> &cs)
		: cs(*cs), lock(*cs)
	{
	}

	PplCsLock::~PplCsLock() {
	}

	void PplCsLock::Lock() {
		this->cs.lock();
	}

	void PplCsLock::Unlock() {
		this->cs.unlock();
	}
}