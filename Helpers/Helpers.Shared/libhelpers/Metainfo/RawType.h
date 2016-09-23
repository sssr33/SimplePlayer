#pragma once

#include <memory>

template<class T>
class RawType{
public:
	typedef T Type;
};

template<class T>
class RawType<T*>{
public:
	typedef T Type;
};

template<class T>
class RawType<std::unique_ptr<T>>{
public:
	typedef T Type;
};

template<class T>
class RawType<std::shared_ptr<T>>{
public:
	typedef T Type;
};