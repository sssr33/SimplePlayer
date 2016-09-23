#pragma once
#include "config.h"
#include "HResultException.h"
#include "Metainfo\Signature.h"

#include <cstdint>
#include <string>
#include <future>
#include <memory>
#include <functional>
#include <assert.h>
#include <exception>

#ifdef HAVE_VISUAL_STUDIO
#include <intrin.h>
#include <windows.h>
#endif

namespace H {
	class System {
	public:
		static void DebuggerBreak(wchar_t* CallingFuncName = nullptr);
		static void Assert(bool expression, wchar_t* CallingFuncName = nullptr);
		static void DebugOutput(const std::string &v);
		static std::wstring FixPathWinRT(const std::wstring &v);
		static void ThrowIfFailed(HRESULT hr, wchar_t* CallingFuncName = nullptr);

		static std::vector<uint8_t> LoadPackageFile(const std::string &path);
		static std::vector<uint8_t> LoadPackageFile(const std::wstring &path);
		static std::wstring GetPackagePath();
	};
}