#pragma once

#include <memory>

class PointerGetter{
public:
	template<class T>
	static T *Get(T *v){
		return v;
	}

	template<class T>
	static T *Get(T &v){
		return &v;
	}

	template<class T>
	static T *Get(std::unique_ptr<T> &v){
		return v.get();
	}

	template<class T>
	static T *Get(std::shared_ptr<T> &v){
		return v.get();
	}
};