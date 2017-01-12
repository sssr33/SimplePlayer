#include "critical_section.h"
#include "..\HSystem.h"

#include <sstream>
#include <thread>

namespace thread {
	// default value of spinCount - https://msdn.microsoft.com/en-us/library/windows/desktop/ms683477%28v=vs.85%29.aspx
	critical_section::critical_section(DWORD spinCount) {
		InitializeCriticalSectionEx(&this->cs, spinCount, 0);
	}

	critical_section::~critical_section() {
		DeleteCriticalSection(&this->cs);
	}

	void critical_section::lock() {
		EnterCriticalSection(&this->cs);
#ifdef _DEBUG
		this->own();
#endif // _DEBUG
	}

	void critical_section::unlock() {
#ifdef _DEBUG
		this->unown();
#endif // _DEBUG

		LeaveCriticalSection(&this->cs);
	}

#ifdef _DEBUG
	bool critical_section::owned() const {
		std::stringstream ss;
		ss << std::this_thread::get_id();

		bool res = this->ownerId == ss.str();
		return res;
	}
#endif // _DEBUG

#ifdef _DEBUG
	void critical_section::own() {
		// to prevent recursive locking
		hAssert(!this->owned());

		std::stringstream ss;
		ss << std::this_thread::get_id();

		this->ownerId = ss.str();
	}

	void critical_section::unown() {
		this->ownerId = std::string();
	}
#endif // _DEBUG
}