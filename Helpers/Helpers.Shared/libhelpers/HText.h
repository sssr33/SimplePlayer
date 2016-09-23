#pragma once
#include "config.h"
#include "ToString.h"

#include <vector>
#include <string>

#ifdef HAVE_VISUAL_STUDIO
#include <Windows.h>
#endif

namespace H {
	class Text {
	public:
		static std::string UriDecode(const void *src, size_t length);

		static std::string UriEncode(const void *src, size_t length);

		static std::wstring ConvertUTF8ToWString(const std::string &utf8);

		static std::string ConvertWStringToUTF8(const std::wstring &s);

		static void BreakNetUrl(const std::wstring &url, std::wstring *protocol, std::wstring *name, std::wstring *port, std::wstring *path);

		template<class T> static std::string ToString(const T &v) {
			return ToStr::Do(v);
		}
	};
}