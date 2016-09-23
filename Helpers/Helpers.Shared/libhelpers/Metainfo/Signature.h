#pragma once

#include <type_traits>

template<class T>
class SigHelper{
};

template<class R, class T>
class SigHelper<R(T::*)() const>{
public:
	typedef R R;
	typedef T T;
	typedef void A1;
	typedef void A2;
};

template<class R, class T, class A1>
class SigHelper<R(T::*)(A1) const>{
public:
	typedef R R;
	typedef T T;
	typedef A1 A1;
	typedef void A2;
};

template<class R, class T, class A1, class A2>
class SigHelper<R(T::*)(A1, A2) const>{
public:
	typedef R R;
	typedef T T;
	typedef A1 A1;
	typedef A2 A2;
};

template<class T>
class Signature{
public:
	typedef typename SigHelper<decltype(&T::operator())>::R R;
	typedef typename SigHelper<decltype(&T::operator())>::A1 A1;
	typedef typename SigHelper<decltype(&T::operator())>::A2 A2;
};

template<class R>
class Signature<R()>{
public:
	typedef R R;
	typedef void A1;
	typedef void A2;
};

template<class R, class A1>
class Signature<R(A1)>{
public:
	typedef R R;
	typedef A1 A1;
	typedef void A2;
};

template<class R, class A1, class A2>
class Signature<R(A1, A2)>{
public:
	typedef R R;
	typedef A1 A1;
	typedef A2 A2;
};

// Fix for 8.1
/*
This may be a compiler bug on windows 8.1, visual studio 2013, c++ v120
This specializtions should work fine, tested once.
To test more use function pointer types with Signature class.
To reproduce compiler bug comment "Fix for 8.1" specializations
and try next code:

template<class T>
typename Signature<T>::A1 F(T t) {
typename Signature<T>::A1 asd = 2.2;
return asd;
}

...

F([](double d){});
*/
template<class R>
class Signature<R(*)()> {
public:
	typedef R R;
	typedef void A1;
	typedef void A2;
};

template<class R, class A1>
class Signature<R(*)(A1)> {
public:
	typedef R R;
	typedef A1 A1;
	typedef void A2;
};

template<class R, class A1, class A2>
class Signature<R(*)(A1, A2)> {
public:
	typedef R R;
	typedef A1 A1;
	typedef A2 A2;
};
// Fix for 8.1