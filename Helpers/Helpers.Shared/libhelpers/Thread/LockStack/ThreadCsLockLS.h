#pragma once
#include "ILockLS.h"
#include "..\critical_section.h"

#include <memory>

namespace thread {
	class ThreadCsLock : protected ILock {
	public:
		ThreadCsLock(thread::critical_section &cs);
		ThreadCsLock(std::unique_ptr<thread::critical_section> &cs);
		ThreadCsLock(std::shared_ptr<thread::critical_section> &cs);
		virtual ~ThreadCsLock();

	protected:
		void Lock() override;
		void Unlock() override;

	private:
		thread::critical_section &cs;
		thread::critical_section::scoped_lock lock;
	};
}