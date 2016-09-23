#pragma once
#include "Macros.h"

#include <Windows.h>

template<class Deleter>
class UniqueHandleBase {
public:
	NO_COPY(UniqueHandleBase);

	UniqueHandleBase(HANDLE handle)
		: handle(handle)
	{
	}

	UniqueHandleBase(UniqueHandleBase &&other)
		: handle(other.handle)
	{
		other.handle = INVALID_HANDLE_VALUE;
	}

	~UniqueHandleBase() {
		if (this->handle != INVALID_HANDLE_VALUE) {
			Deleter del;
			del(this->handle);
		}
	}

	UniqueHandleBase &operator=(UniqueHandleBase &&other) {
		if (this != &other) {
			this->handle = other.handle;

			other.handle = INVALID_HANDLE_VALUE;
		}

		return *this;
	}
	
	HANDLE get() const {
		return this->handle;
	}

private:
	HANDLE handle;
};

struct UniqueHandleDeleter {
	void operator()(HANDLE handle);
};

struct UniqueFindDeleter {
	void operator()(HANDLE handle);
};

typedef UniqueHandleBase<UniqueHandleDeleter> UniqueHandle;
typedef UniqueHandleBase<UniqueFindDeleter> UniqueFind;