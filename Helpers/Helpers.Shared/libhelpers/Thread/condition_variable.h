#pragma once
#include "critical_section.h"
#include "..\Macros.h"

#include <Windows.h>

namespace thread {
	class condition_variable {
	public:
		NO_COPY_MOVE(condition_variable);

		condition_variable() {
			InitializeConditionVariable(&this->cv);
		}

		~condition_variable() {
		}

		void wait(critical_section &cs, DWORD milliseconds = INFINITE) {
			SleepConditionVariableCS(&this->cv, &cs.cs, milliseconds);
		}

		void notify() {
			WakeConditionVariable(&this->cv);
		}

		void notify_all() {
			WakeAllConditionVariable(&this->cv);
		}

	private:
		CONDITION_VARIABLE cv;
	};
}