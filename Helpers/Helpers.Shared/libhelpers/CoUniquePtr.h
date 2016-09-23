#pragma once
#include "config.h"

#ifdef HAVE_VISUAL_STUDIO
#include <windows.h>

template<class T>
struct CoDeleter{
	void operator()(T *ptr){
		CoTaskMemFree(ptr);
	}
};

template<class T, class Deleter>
class AddressOfHelper{
public:
	AddressOfHelper(std::unique_ptr<T, Deleter> &tmp)
		: tmp(tmp){
	}

	~AddressOfHelper(){
		this->tmp.reset(this->ptr);
	}

	operator T**(){
		return &this->ptr;
	}

private:
	std::unique_ptr<T, Deleter> &tmp;
	T *ptr;
};

// Unique pointer for objects that were allocated with CoTaskMemAlloc
template<class T> struct CoUniquePtr : public std::unique_ptr < T, CoDeleter<T> > {
	AddressOfHelper<T, CoDeleter<T>> GetAddressOf(){
		return AddressOfHelper<T, CoDeleter<T>>(*this);
	}
};

#endif