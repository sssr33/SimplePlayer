#include "TypeConverter.h"

std::wstring TypeConverter<std::wstring, std::string>::Convert(const std::string &v) {
	std::wstring tmp(v.begin(), v.end());
	return tmp;
}

std::string TypeConverter<std::string, std::wstring>::Convert(const std::wstring &v) {
	std::string tmp(v.begin(), v.end());
	return tmp;
}

D2D1_SIZE_F TypeConverter<D2D1_SIZE_F, D2D1_SIZE_U>::Convert(const D2D1_SIZE_U &v) {
	D2D1_SIZE_F tmp;

	tmp.width = (float)v.width;
	tmp.height = (float)v.height;

	return tmp;
}

#if HAVE_WINRT == 1
Windows::UI::Color TypeConverter<Windows::UI::Color, D2D1::ColorF>::Convert(const D2D1::ColorF &v) {
	Windows::UI::Color color;

	color.A = (uint8_t)(v.a * 255.0f);
	color.R = (uint8_t)(v.r * 255.0f);
	color.G = (uint8_t)(v.g * 255.0f);
	color.B = (uint8_t)(v.b * 255.0f);

	return color;
}

D2D1::ColorF TypeConverter<D2D1::ColorF, Windows::UI::Color>::Convert(const Windows::UI::Color &v) {
	const float Scale = 1.0f / 255.0f;
	float a, r, g, b;

	a = (float)v.A * Scale;
	r = (float)v.R * Scale;
	g = (float)v.G * Scale;
	b = (float)v.B * Scale;

	auto compColor = Windows::UI::Colors::Transparent;
	if (v.A == compColor.A && v.R == compColor.R && v.G == compColor.G && v.B == compColor.B) {
		a = r = g = b = 0.0f;
	}

	return D2D1::ColorF(r, g, b, a);
};

Windows::UI::Color TypeConverter<Windows::UI::Color, D2D1_COLOR_F>::Convert(const D2D1_COLOR_F &v) {
	D2D1::ColorF tmp(v.r, v.g, v.b, v.a);
	return TypeConverter<Windows::UI::Color, D2D1::ColorF>::Convert(tmp);
}

D2D1_COLOR_F TypeConverter<D2D1_COLOR_F, Windows::UI::Color>::Convert(const Windows::UI::Color &v) {
	D2D1_COLOR_F res = TypeConverter<D2D1::ColorF, Windows::UI::Color>::Convert(v);
	return res;
}

D2D1_COLOR_F TypeConverter<D2D1_COLOR_F, D2D1::ColorF>::Convert(const D2D1::ColorF &v) {
	D2D1_COLOR_F color;

	color.a = v.a;
	color.r = v.r;
	color.g = v.g;
	color.b = v.b;

	return color;
}

D2D1::ColorF TypeConverter<D2D1::ColorF, D2D1_COLOR_F>::Convert(const D2D1_COLOR_F &v) {
	D2D1::ColorF color(v.r, v.g, v.b, v.a);
	return color;
}

Platform::String ^TypeConverter<Platform::String ^, std::wstring>::Convert(const std::wstring &v) {
	Platform::String ^tmp = ref new Platform::String(v.data(), (int)v.size());
	return tmp;
}

std::wstring TypeConverter<std::wstring, Platform::String ^>::Convert(Platform::String ^v) {
	std::wstring tmp(v->Data(), v->Length());
	return tmp;
}

Platform::String ^TypeConverter<Platform::String ^, std::string>::Convert(const std::string &v) {
	std::wstring tmpWStr(v.begin(), v.end());
	Platform::String ^tmp = ref new Platform::String(tmpWStr.data(), (int)tmpWStr.size());
	return tmp;
}

std::string TypeConverter<std::string, Platform::String ^>::Convert(Platform::String ^v) {
	std::wstring tmpWStr(v->Data(), v->Length());
	std::string tmp(tmpWStr.begin(), tmpWStr.end());
	return tmp;
}

Windows::Foundation::TimeSpan TypeConverter<Windows::Foundation::TimeSpan, int64_t>::Convert(const int64_t &v) {
	Windows::Foundation::TimeSpan res;
	res.Duration = v;
	return res;
}

int64_t TypeConverter<int64_t, Windows::Foundation::TimeSpan>::Convert(Windows::Foundation::TimeSpan v) {
	return v.Duration;
}
#endif