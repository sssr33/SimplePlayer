#pragma once

namespace thread {
	class ILockListItem {
	public:
		ILockListItem();
		virtual ~ILockListItem();

		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		ILockListItem *Prev;
		ILockListItem *Next;
	};
}