#pragma once
#include "ThreadTask.h"

template<class T>
class ThreadTaskBase{
public:
	ThreadTaskBase(){
	}

	virtual ~ThreadTaskBase(){
	}

	virtual void Run() override{

	}
};