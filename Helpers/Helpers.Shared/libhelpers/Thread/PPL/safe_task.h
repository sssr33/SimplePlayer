#pragma once
#include <ppltasks.h>
#include <mutex>
#include <condition_variable>

namespace Concurrency{
	template<class T>
	class safe_task{
	public:
		std::mutex waitMutex;
		std::condition_variable waitConditionVariable;
		bool waitReady = false;

		safe_task()
			: valid(false){

		}
		safe_task(concurrency::task<T> &&t)
			: valid(true), t(std::move(t)){
		}

		~safe_task(){
			if (this->valid) {
				std::unique_lock<std::mutex> tempLock(this->waitMutex);
				//while (!this->waitReady) {
				waitConditionVariable.wait(tempLock, [&] {return this->waitReady; });
				//};
			}
		}

		safe_task &operator=(concurrency::task<T> &&t){
			this->valid = true;
			this->t = std::move(t);

			return *this;
		}

		concurrency::task<T> *operator->(){
			return &this->t;
		}

		const concurrency::task<T> *operator->() const{
			return &this->t;
		}

		operator concurrency::task<T>*(){
			return &this->t;
		}

		operator const concurrency::task<T>*() const{
			return &this->t;
		}

		operator bool(){
			return this->valid;
		}

		void wait(){
			std::unique_lock<std::mutex> tempLock(this->waitMutex);
//			while (!this->waitReady) {
				waitConditionVariable.wait(tempLock, [&] {return this->waitReady;});
//			};
		}
	private:
		bool valid;
		concurrency::task<T> t;
	};
}