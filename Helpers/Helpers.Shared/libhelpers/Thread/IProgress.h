#pragma once
#include "libhelpers\Metainfo\Signature.h"

#include <functional>

namespace thread {
	// class T - type of progress data
	template<class T>
	class IProgress {
	public:
		IProgress() {}
		virtual ~IProgress() {}

		virtual void Report(const T &progress) = 0;
	};

	// class T - type of progress data
	// class Fn - type of handling object(std::function, function pointer), void(T);
	// Use MakeGenericProgress function to create IGenericProgress instance
	template<class T, class Fn>
	class IGenericProgress : public IProgress<T> {
	public:
		IGenericProgress(Fn fn) 
			: fn(fn)
		{
		}

		virtual ~IGenericProgress() {}

		void Report(const T &progress) override {
			fn(progress);
		}

	private:
		Fn fn;
	};

	template<class Fn>
	IGenericProgress<typename Signature<Fn>::A1, Fn> MakeGenericProgress(Fn fn) {
		return IGenericProgress<typename Signature<Fn>::A1, Fn>(fn);
	}
}