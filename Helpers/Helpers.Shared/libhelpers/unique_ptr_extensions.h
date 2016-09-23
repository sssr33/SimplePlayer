#pragma once

#include <memory>

template<class T, class D>
class GetAddressOfUnique {
public:
	GetAddressOfUnique(std::unique_ptr<T, D> &ptr)
		: ptr(ptr)
	{
		this->rawPtr = this->ptr.release();
	}

	operator T **() {
		return &this->rawPtr;
	}

	~GetAddressOfUnique() {
		this->ptr.reset(this->rawPtr);
	}

private:
	T *rawPtr;
	std::unique_ptr<T, D> &ptr;
};

template<class T, class D>
GetAddressOfUnique<T, D> GetAddressOf(std::unique_ptr<T, D> &v) {
	return GetAddressOfUnique<T, D>(v);
}

template<class T, class D>
std::unique_ptr<T, D> WrapUnique(T *ptr, const D &deleter) {
	return std::unique_ptr<T, D>(ptr, deleter);
}