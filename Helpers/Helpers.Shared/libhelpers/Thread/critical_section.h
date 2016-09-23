#pragma once
#include "..\Macros.h"

#include <Windows.h>
#include <chrono>
#include <memory>

namespace thread {
	class condition_variable;

	class critical_section {
	public:
		NO_COPY_MOVE(critical_section);

		// default value of spinCount - https://msdn.microsoft.com/en-us/library/windows/desktop/ms683477%28v=vs.85%29.aspx
		critical_section(DWORD spinCount = 4000) {
			InitializeCriticalSectionEx(&this->cs, spinCount, 0);
		}

		~critical_section() {
			DeleteCriticalSection(&this->cs);
		}

		void lock() {
			EnterCriticalSection(&this->cs);
		}

		void unlock() {
			LeaveCriticalSection(&this->cs);
		}

	private:
		CRITICAL_SECTION cs;

		friend class condition_variable;

	public:

		template <bool b> struct lock_yielder {
			lock_yielder() {}
		};

		template <> struct lock_yielder<true> {
			lock_yielder() {
				// http://stackoverflow.com/questions/17325888/c11-thread-waiting-behaviour-stdthis-threadyield-vs-stdthis-thread
				// http://stackoverflow.com/questions/3992715/critical-section-problem-in-windows-7
				//std::this_thread::sleep_for(std::chrono::nanoseconds(1)); // ok, nanotechnology is on duty
				Sleep(1); // almost ok, but this is a whole 1 millisecond! UPD looks like this works better
				//std::this_thread::yield(); // a bit better
				//Sleep(0); // - better
				// no wait - works really bad for fast lock/unlock scenarios
			}
		};

		template<bool yielding>
		class scoped_lock_base {
		public:
			scoped_lock_base()
				: cs(nullptr) {
			}

			scoped_lock_base(critical_section &cs)
				: cs(&cs) {
				this->cs->lock();
			}

			scoped_lock_base(std::unique_ptr<critical_section> &cs)
				: cs(cs.get()) {
				this->cs->lock();
			}

			scoped_lock_base(std::shared_ptr<critical_section> &cs)
				: cs(cs.get()) {
				this->cs->lock();
			}

			scoped_lock_base(const scoped_lock_base &other) = delete;

			scoped_lock_base(scoped_lock_base &&other)
				: cs(other.cs)
			{
				other.cs = nullptr;
			}

			~scoped_lock_base() {
				if (this->cs) {
					this->cs->unlock();
					lock_yielder<yielding> yielder; // performing yielding logic
				}
			}

			scoped_lock_base &operator=(const scoped_lock_base &other) = delete;

			scoped_lock_base &operator=(scoped_lock_base &&other) {
				if (this != &other) {
					this->cs = other.cs;
					other.cs = nullptr;
				}

				return *this;
			}
		private:
			critical_section *cs;
		};

		// for common use
		typedef scoped_lock_base<false> scoped_lock;

		/* 
		for use in sections with fast can occur
		example of such section:
		1   while(true){
		2     // no work at this line...
		3   scoped_yield_lock lk(cs);
		4   some work...
		      // no work after lk is out of scope...
		      // because after 4th line we will go to next iteration of while(i.e. 1st line)
		      // here is a *very small* period of time so scoped_lock can be loked by *this* thread
		      // while other thread can wait for a long time
			  // http://stackoverflow.com/questions/3992715/critical-section-problem-in-windows-7
		5   }
		*/
		typedef scoped_lock_base<true> scoped_yield_lock;
	};
}