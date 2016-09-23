#pragma once

#include <functional>
#include <memory>
#include <list>

template<class Arg1>
class ObservableBase;

template<class Arg1>
struct ObservableToken {
	typedef std::function<void(Arg1)> FnType;

	typename std::list<std::weak_ptr<FnType>>::iterator iterator;
	std::shared_ptr<FnType> sharedFn;
	ObservableBase<Arg1> *observable;
};