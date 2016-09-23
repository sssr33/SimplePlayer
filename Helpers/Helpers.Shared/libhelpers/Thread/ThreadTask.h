#pragma once

class ThreadTask{
public:
	ThreadTask(){
	}

	virtual ~ThreadTask(){
	}

	virtual void Run() = 0;
};