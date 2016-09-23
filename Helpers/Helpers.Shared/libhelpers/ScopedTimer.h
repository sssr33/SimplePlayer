#pragma once
#include "Timer.h"

class ScopedTimer{
public:
	ScopedTimer(Timer::timer_id id, Timer &t)
		: timer(t), id(id){
	}

	~ScopedTimer(){
		timer.destroy(id);
	}

private:
	Timer::timer_id id;
	Timer &timer;
};