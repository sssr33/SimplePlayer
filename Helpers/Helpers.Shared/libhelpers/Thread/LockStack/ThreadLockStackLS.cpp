#include "ThreadLockStackLS.h"
#include "libhelpers\HSystem.h"

namespace thread {
	LockList::LockList() 
		: current(nullptr)
	{
	}

	ThreadLockStack::ThreadLockStack() 
		: maxLockListCount(32) // up to 32 simultaneous threads can [re]use lock lists
	{
		this->afterLast = &this->dummyAfterLast;
		this->beforeBegin = &this->dummyBeforeBegin;
	}

	void ThreadLockStack::Add(ILockListItem *lock) {
		auto lockList = this->GetLockListLocked();

		if (!lockList->first && !lockList->last) {
			// initialize list
			lockList->first = lock;
			lockList->last = lock;
			lockList->current = lock;

			lock->Prev = this->beforeBegin;
			lock->Next = this->afterLast;
		}
		else if (lockList->first && lockList->last) {
			// append lock to end of lockList
			// lockList.last <-- lock
			lock->Prev = lockList->last;
			// lockList.last --> lock
			lockList->last->Next = lock;
			// lockList.last_OLD <--> lockList.last
			lockList->last = lock;
			lockList->last->Next = this->afterLast;

			if (lockList->current == this->afterLast) {
				// new element was added to end
				// update current if it was pointing to afterLast
				lockList->current = lock;
			}
		}
		else {
			// malformed list
			H::System::Assert(false);
		}
	}

	void ThreadLockStack::Remove(ILockListItem *lock) {
		auto threadId = std::this_thread::get_id();
		auto lockList = this->GetLockListLocked(threadId);

		// Check order in which locks created on stack
		// Maybe need to implement remove at custom position
		H::System::Assert(lockList->last == lock);

		if (lockList->first == lockList->last) {
			// empty list
			lockList->first = lockList->last = nullptr;
			lockList->current = nullptr;

			{
				thread::critical_section::scoped_lock lk(this->threadLocksCs);
				if (this->threadLocks.size() > this->maxLockListCount) {
					this->threadLocks.erase(threadId);
				}
			}
		}
		else {
			if (lockList->current == lockList->last) {
				// this can happen in such situation:
				// current--------------------current
				// lock0----lock1---unlock0---lock2
				// lock2 is the begining of new section
				// if lock2 will be removed then this code will be invoked
				// in order to keep begining of new section(current) up to date
				// we need to set it to afterLast to update it in next Add after unlock0
				lockList->current = this->afterLast;
			}

			// remove 1 element from back
			lockList->last = lockList->last->Prev;
			lockList->last->Next = this->afterLast;
		}

		lock->Prev = lock->Next = nullptr;
	}

	void ThreadLockStack::Lock(const LockRange &range) {
		if (range.first && range.last) {
			auto lk = range.first;
			while (lk != range.last->Next) {
				lk->Lock();
				lk = lk->Next;
			}

			LockList *lockList = this->GetLockListLocked();

			lockList->current = range.first;
		}
	}

	LockRange ThreadLockStack::UnlockRange() {
		LockRange range;
		LockList *lockList = this->GetLockListLocked();

		if (lockList->current && lockList->last) {
			// |first-----------------------------------------------last
			// |current---last->|next(null)
			// |         unlock |
			// |                |current--------last->|next(null)
			// |                               unlock |
			// |                                      |current------last
			range.first = lockList->current;
			range.last = lockList->last;

			// set new current as element after last element(can be null)
			// element after last element - 1st element of next LockRange
			lockList->current = lockList->last->Next;

			auto lk = range.last;
			while (lk != range.first->Prev) {
				lk->Unlock();
				lk = lk->Prev;
			}
		}

		return range;
	}

	uint32_t ThreadLockStack::GetMaxLockListCount() const {
		thread::critical_section::scoped_lock lk(this->threadLocksCs);
		return this->maxLockListCount;
	}

	void ThreadLockStack::SetMaxLockListCount(uint32_t v) {
		thread::critical_section::scoped_lock lk(this->threadLocksCs);
		this->maxLockListCount = v;
	}

	LockList *ThreadLockStack::GetLockListLocked() {
		// http://stackoverflow.com/questions/6438086/iterator-invalidation-rules
		// std map iterators are stable so we can hold threadLocksCs only for GetLockList
		// because each thread have it's own LockList

		thread::critical_section::scoped_lock lk(this->threadLocksCs);
		return this->GetLockList();
	}

	LockList *ThreadLockStack::GetLockListLocked(const std::thread::id &id) {
		// http://stackoverflow.com/questions/6438086/iterator-invalidation-rules
		// std map iterators are stable so we can hold threadLocksCs only for GetLockList
		// because each thread have it's own LockList

		thread::critical_section::scoped_lock lk(this->threadLocksCs);
		return this->GetLockList(id);
	}

	LockList *ThreadLockStack::GetLockList() {
		return this->GetLockList(std::this_thread::get_id());
	}

	LockList *ThreadLockStack::GetLockList(const std::thread::id &id) {
		LockList *list;

		auto find = this->threadLocks.find(id);
		if (find != this->threadLocks.end()) {
			list = &find->second;
		}
		else {
			auto addRes = this->threadLocks.insert(std::make_pair(id, LockList()));
			list = &addRes.first->second;
		}

		return list;
	}

	ThreadLockStack::DummyLockListItem::DummyLockListItem() {
	}

	ThreadLockStack::DummyLockListItem::~DummyLockListItem() {
	}

	void ThreadLockStack::DummyLockListItem::Lock() {
	}

	void ThreadLockStack::DummyLockListItem::Unlock() {
	}

	ThreadLockStack *ThreadLockStackSingleton::GetInstance() {
		static ThreadLockStack lockStack;
		return &lockStack;
	}
}