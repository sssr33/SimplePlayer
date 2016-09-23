#pragma once

#include <Windows.h>
#include <chrono>

class StopWatch {
	template<class... Args> struct ElapsedHelper {
		typedef void RetType;
		static RetType Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) { static_assert(false, "Not implemented"); }
	};

	template<class T, intmax_t N, intmax_t D>
	struct ElapsedHelper<std::chrono::duration<T, std::ratio<N, D>>> {
		typedef std::chrono::duration<T, std::ratio<N, D>> RetType;
		static RetType Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) {
			T durationVal = (T)(time.QuadPart * D) / (T)(frequency.QuadPart * N);
			return RetType(durationVal);
		}
	};

	template<intmax_t N, intmax_t D>
	struct ElapsedHelper<std::ratio<N, D>> {
		typedef std::chrono::duration<int64_t, std::ratio<N, D>> RetType;
		static RetType Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) {
			int64_t durationVal = (int64_t)(time.QuadPart * D) / (int64_t)(frequency.QuadPart * N);
			return RetType(durationVal);
		}
	};

	template<class T, intmax_t N, intmax_t D>
	struct ElapsedHelper<T, std::ratio<N, D>> {
		typedef std::chrono::duration<T, std::ratio<N, D>> RetType;
		static RetType Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) {
			T durationVal = (T)(time.QuadPart * D) / (T)(frequency.QuadPart * N);
			return RetType(durationVal);
		}
	};

	template<>
	struct ElapsedHelper<double> {
		typedef double RetType;
		static double Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) {
			// return seconds
			double durationVal = (double)(time.QuadPart) / (double)(frequency.QuadPart);
			return durationVal;
		}
	};

	template<>
	struct ElapsedHelper<float> {
		typedef float RetType;
		static float Get(const LARGE_INTEGER &time, const LARGE_INTEGER &frequency) {
			// return seconds
			float durationVal = (float)(time.QuadPart) / (float)(frequency.QuadPart);
			return durationVal;
		}
	};

public:
	StopWatch() {
		this->prevTime.QuadPart = 0;
		QueryPerformanceFrequency(&this->frequency);
	}

	float Elapsed() const {
		return this->Elapsed<float>();
	}

	template<class... Args> typename ElapsedHelper<Args...>::RetType Elapsed() const {
		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);
		time.QuadPart = time.QuadPart - this->prevTime.QuadPart;

		return ElapsedHelper<Args...>::Get(time, this->frequency);
	}

	void Start() {
		QueryPerformanceCounter(&this->prevTime);
	}

private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER prevTime;
};