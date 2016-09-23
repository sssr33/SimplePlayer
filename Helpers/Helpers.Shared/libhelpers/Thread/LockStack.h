#pragma once
#include "LockStack\ThreadCsLockLS.h"
#include "LockStack\PplCsLockLS.h"
#include "LockStack\StdMutexLockLS.h"
#include "LockStack\LockRangeLS.h"

namespace thread {
	class LockStack {
	public:
		class ScopedUnlock {
		public:
			ScopedUnlock();
			~ScopedUnlock();
		private:
			LockRange lockRange;
		};
	};
}