#include "ThreadCsLockLS.h"

namespace thread {
	ThreadCsLock::ThreadCsLock(thread::critical_section &cs)
		: cs(cs), lock(cs)
	{
	}

	ThreadCsLock::ThreadCsLock(std::unique_ptr<thread::critical_section> &cs)
		: cs(*cs), lock(cs)
	{
	}

	ThreadCsLock::ThreadCsLock(std::shared_ptr<thread::critical_section> &cs)
		: cs(*cs), lock(cs)
	{
	}

	ThreadCsLock::~ThreadCsLock() {
	}

	void ThreadCsLock::Lock() {
		this->cs.lock();
	}

	void ThreadCsLock::Unlock() {
		this->cs.unlock();
	}
}