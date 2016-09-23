#pragma once
#include "HTime.h"

template<class T>
struct rational{
	static const rational<T> HNSUnits;

	rational()
		: num(1), den(1){
	}

	rational(const T &num, const T &den)
		: num(num), den(den){
	}

	T convertValueTo(const T &value, const rational<T> &other) const{
		T res = (this->num * other.den * value) / (this->den * other.num);
		return res;
	}

	T num;
	T den;
};

template<class T>
const rational<T> rational<T>::HNSUnits(static_cast<T>(1), static_cast<T>(H::Time::HNSResolution));