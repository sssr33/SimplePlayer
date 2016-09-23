#pragma once
#include "ILockListItemLS.h"
#include "LockRangeLS.h"
#include "..\critical_section.h"
#include "libhelpers\Macros.h"

#include <map>
#include <thread>

namespace thread {
	struct LockList : public LockRange {
		// used as begin marker for first lock after previous usage of ThreadLockStack::LockRange
		ILockListItem *current;

		LockList();
	};

	class ThreadLockStack {
	public:
		ThreadLockStack();

		NO_COPY_MOVE(ThreadLockStack);

		void Add(ILockListItem *lock);
		void Remove(ILockListItem *lock);

		void Lock(const LockRange &range);
		LockRange UnlockRange();

		uint32_t GetMaxLockListCount() const;
		void SetMaxLockListCount(uint32_t v);

	private:
		mutable thread::critical_section threadLocksCs;
		std::map<std::thread::id, LockList> threadLocks;
		uint32_t maxLockListCount;

		LockList *GetLockListLocked();
		LockList *GetLockListLocked(const std::thread::id &id);
		LockList *GetLockList();
		LockList *GetLockList(const std::thread::id &id);

		class DummyLockListItem : public ILockListItem {
		public:
			DummyLockListItem();
			virtual ~DummyLockListItem();

			void Lock() override;
			void Unlock() override;
		};

		DummyLockListItem dummyAfterLast;
		DummyLockListItem dummyBeforeBegin;
		DummyLockListItem *afterLast;
		DummyLockListItem *beforeBegin;
	};

	class ThreadLockStackSingleton {
	public:
		static ThreadLockStack *GetInstance();
	};
}