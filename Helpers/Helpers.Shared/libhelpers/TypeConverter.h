#pragma once
#include "config.h"

#include <d2d1.h>
#include <string>
#include <cstdint>

template<class D, class S>
struct TypeConverter {
	static D Convert(const S &v) {
		static_assert(false, "Not implemented");
		return D();
	}
};

// std::wstring <-> std::string
template<> struct TypeConverter<std::wstring, std::string> {
	static std::wstring Convert(const std::string &v);
};

template<> struct TypeConverter<std::string, std::wstring> {
	static std::string Convert(const std::wstring &v);
};

// D2D1_SIZE_U -> D2D1_SIZE_F
template<> struct TypeConverter<D2D1_SIZE_F, D2D1_SIZE_U> {
	static D2D1_SIZE_F Convert(const D2D1_SIZE_U &v);
};

#if HAVE_WINRT == 1
// Windows::UI::Color <-> D2D1::ColorF
template<> struct TypeConverter<Windows::UI::Color, D2D1::ColorF> {
	static Windows::UI::Color Convert(const D2D1::ColorF &v);
};

template<> struct TypeConverter<D2D1::ColorF, Windows::UI::Color> {
	static D2D1::ColorF Convert(const Windows::UI::Color &v);
};

// Windows::UI::Color <-> D2D1_COLOR_F
template<> struct TypeConverter<Windows::UI::Color, D2D1_COLOR_F> {
	static Windows::UI::Color Convert(const D2D1_COLOR_F &v);
};

template<> struct TypeConverter<D2D1_COLOR_F, Windows::UI::Color> {
	static D2D1_COLOR_F Convert(const Windows::UI::Color &v);
};

// D2D1_COLOR_F <-> D2D1::ColorF
template<> struct TypeConverter<D2D1_COLOR_F, D2D1::ColorF> {
	static D2D1_COLOR_F Convert(const D2D1::ColorF &v);
};

template<> struct TypeConverter<D2D1::ColorF, D2D1_COLOR_F> {
	static D2D1::ColorF Convert(const D2D1_COLOR_F &v);
};

// Platform::String ^ <-> std::wstring
template<> struct TypeConverter<Platform::String ^, std::wstring> {
	static Platform::String ^Convert(const std::wstring &v);
};

template<> struct TypeConverter<std::wstring, Platform::String ^> {
	static std::wstring Convert(Platform::String ^v);
};

// Platform::String ^ <-> std::string
template<> struct TypeConverter<Platform::String ^, std::string> {
	static Platform::String ^Convert(const std::string &v);
};

template<> struct TypeConverter<std::string, Platform::String ^> {
	static std::string Convert(Platform::String ^v);
};

// Windows::Foundation::TimeSpan <-> int64_t
template<> struct TypeConverter<Windows::Foundation::TimeSpan, int64_t> {
	static  Windows::Foundation::TimeSpan Convert(const int64_t &v);
};

template<> struct TypeConverter<int64_t, Windows::Foundation::TimeSpan> {
	static int64_t Convert(Windows::Foundation::TimeSpan v);
};
#endif