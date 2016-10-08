#pragma once

class ThreadTask{
public:
	ThreadTask(){
	}

	virtual ~ThreadTask(){
	}

	virtual void Run() = 0;
};

// T - callable type(lambda, function pointer)
template<class T>
class GenericThreadTask : public ThreadTask {
public:
	GenericThreadTask(T callable) 
		: callable(callable)
	{}

	virtual ~GenericThreadTask() {}

	void Run() override {
		this->callable();
	}

private:
	T callable;
};

template<class T>
GenericThreadTask<T> *MakeGenericThreadTask(T callable) {
	return new GenericThreadTask<T>(callable);
}