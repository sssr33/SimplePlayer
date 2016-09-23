#pragma once
#include "Macros.h"

// D - void(*)(T*);
// TODO check is valid can be substituted by T invalidValue or like that
// invalidValue - value of the type T which must not to be deleted by destructor
// examples of invalidValue:
// pointer - nullptr
// HANDLE - INVALID_HANDLE_VALUE
template<class T, class D> class ScopedValue {
public:
	NO_COPY(ScopedValue);

	ScopedValue()
		: valid(false) {
	}

	ScopedValue(ScopedValue &&other)
		: val(std::move(other.val)), deleter(std::move(other.deleter)),
		valid(other.valid)
	{
		other.valid = false;
	}

	ScopedValue(const T &v, D deleter)
		: val(v), deleter(deleter), valid(true) {
	}

	ScopedValue(T &&v, D deleter)
		: val(std::move(v)), deleter(deleter), valid(true) {
	}

	~ScopedValue() {
		if (this->valid) {
			this->deleter(&this->val);
		}
	}

	ScopedValue &operator=(ScopedValue &&other) {
		if (this != &other) {
			this->val = std::move(other.val);
			this->deleter = std::move(other.deleter);
			this->valid = other.valid;

			other.valid = false;
		}

		return *this;
	}

	bool Valid() const {
		return this->valid;
	}

	void Invalidate() {
		this->valid = false;
	}

	T *GetPtr() {
		return &this->val;
	}

	T &GetRef() {
		return this->val;
	}

private:
	bool valid;
	T val;
	D deleter;
};

template<class T, class D>
ScopedValue<T, D> MakeScopedValue(const T &v, D deleter) {
	return ScopedValue<T, D>(v, deleter);
}