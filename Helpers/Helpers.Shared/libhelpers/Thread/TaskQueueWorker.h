#pragma once
#include "ThreadTask.h"
#include "critical_section.h"
#include "condition_variable.h"

#include <thread>
#include <memory>
#include <queue>

class TaskQueueWorker {
public:
	TaskQueueWorker();
	~TaskQueueWorker();

	void AddTask(std::unique_ptr<ThreadTask> &&v);

private:
	thread::critical_section cs;
	thread::condition_variable cv;

	bool work;
	std::queue<std::unique_ptr<ThreadTask>> tasks;
	std::thread thread;

	void ThreadProc();
	bool GetNextTask(std::unique_ptr<ThreadTask> &task);
};