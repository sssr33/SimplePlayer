#pragma once
#include "ThreadTask.h"
#include "..\Containers\ConcurrentQueue.h"

#include <memory>
#include <thread>
#include <mutex>
#include <list>
#include <cstdint>
#include <limits>

class ThreadPool : public std::enable_shared_from_this<ThreadPool>{
	class Worker;

	struct this_is_private {
	};
public:
	static std::shared_ptr<ThreadPool> Make(){
		return std::make_shared<ThreadPool>(this_is_private());
	}

	static std::shared_ptr<ThreadPool> Make(uint32_t threadCount){
		return std::make_shared<ThreadPool>(this_is_private(), threadCount);
			}

	explicit ThreadPool(const this_is_private &)
		: wantExit(false){
		uint32_t threadCount = std::thread::hardware_concurrency();
		this->AddWorkers(threadCount);
		}

	explicit ThreadPool(const this_is_private &, uint32_t threadCount)
		: wantExit(false){
		this->AddWorkers(threadCount);
	}

	~ThreadPool(){
		this->wantExit = true;
		this->taskQueue.StopWait();
	}

	std::shared_ptr<ThreadPool> GetPtr() {
		return shared_from_this();
	}

	void AddTask(std::unique_ptr<ThreadTask> &&task){
		this->taskQueue.Push(std::move(task));
	}
private:
	ConcurrentQueue<std::unique_ptr<ThreadTask>> taskQueue;
	std::mutex workersMtx;
	std::list<std::unique_ptr<Worker>> workers;
	bool wantExit;

	void AddWorkers(uint32_t count){
		std::lock_guard<std::mutex> lk(this->workersMtx);

		for (uint32_t i = 0; i < count; i++){
			std::unique_ptr<Worker> worker(new Worker(this));
			worker->Run();
			this->workers.push_back(std::move(worker));
		}
	}

	class Worker{
	public:
		Worker(ThreadPool *parent)
			: parent(parent){
		}

		~Worker(){
			if (this->thread.joinable()){
				this->thread.join();
			}
		}

		void Run(){
			this->thread = std::thread([=](){
				while (!this->parent->wantExit){
					std::unique_ptr<ThreadTask> task;

					if (this->parent->taskQueue.Pop(task, true)){
						task->Run();
					}
				}
			});
		}
	private:
		ThreadPool *parent;
		std::thread thread;
	};
};