#include "TaskQueueWorker.h"

TaskQueueWorker::TaskQueueWorker()
	: work(true),
	thread([=]() { this->ThreadProc(); })
{
}

TaskQueueWorker::~TaskQueueWorker() {
	this->work = false;
	this->cv.notify();

	if (this->thread.joinable()) {
		this->thread.join();
	}
}

void TaskQueueWorker::AddTask(std::unique_ptr<ThreadTask> &&v) {
	{
		thread::critical_section::scoped_lock lk(this->cs);
		this->tasks.push(std::move(v));
	}

	this->cv.notify();
}

void TaskQueueWorker::ThreadProc() {
	std::unique_ptr<ThreadTask> task;

	while (this->GetNextTask(task)) {
		task->Run();
		task = nullptr;
	}
}

bool TaskQueueWorker::GetNextTask(std::unique_ptr<ThreadTask> &task) {
	thread::critical_section::scoped_lock lk(this->cs);

	while (this->work && this->tasks.empty()) {
		this->cv.wait(this->cs);
	}

	if (!this->work) {
		return false;
	}

	task = std::move(this->tasks.front());
	this->tasks.pop();

	return true;
}