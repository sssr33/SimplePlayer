#pragma once
#include "..\config.h"

#include <cstdint>
#include <string>
#include <d2d1.h>

template<class T>
struct ZeroDefaultValue {
	static T Get() {
		static_cast(false, "Not implemented");
	}
};

template<class T>
struct ZeroDefaultValue<const T &> {
	static std::string Get() {
		// ZeroDefaultValue can be used inside other method which will return const T & to value created in this method
		static_assert(false, "Don't return references when using ZeroDefaultValue!");
		return T();
	}
};

template<class T>
struct ZeroDefaultValue<T &> {
	static std::string Get() {
		// ZeroDefaultValue can be used inside other method which will return const T & to value created in this method
		static_assert(false, "Don't return references when using ZeroDefaultValue!");
		return T();
	}
};

template<class T>
struct ZeroDefaultValue<const T *> {
	static std::string Get() {
		return nullptr;
	}
};

template<class T>
struct ZeroDefaultValue<T *> {
	static std::string Get() {
		return nullptr;
	}
};

template<>
struct ZeroDefaultValue<int> {
	static int Get() {
		return 0;
	}
};

template<>
struct ZeroDefaultValue<uint32_t> {
	static uint32_t Get() {
		return 0;
	}
};

template<>
struct ZeroDefaultValue<float> {
	static float Get() {
		return 0.0f;
	}
};

template<>
struct ZeroDefaultValue<bool> {
	static bool Get() {
		return false;
	}
};

template<>
struct ZeroDefaultValue<D2D1::ColorF> {
	static D2D1::ColorF Get() {
		return D2D1::ColorF::Black;
	}
};

template<>
struct ZeroDefaultValue<D2D1_COLOR_F> {
	static D2D1_COLOR_F Get() {
		return D2D1::ColorF(D2D1::ColorF::Black);
	}
};

template<>
struct ZeroDefaultValue<std::string> {
	static std::string Get() {
		return std::string();
	}
};

template<>
struct ZeroDefaultValue<std::wstring> {
	static std::wstring Get() {
		return std::wstring();
	}
};

#if HAVE_WINRT == 1
template<>
struct ZeroDefaultValue<Platform::String ^> {
	static Platform::String ^Get() {
		return nullptr;
	}
};
#endif