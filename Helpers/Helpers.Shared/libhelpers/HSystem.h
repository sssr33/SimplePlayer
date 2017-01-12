#pragma once
#include "config.h"
#include "HResultException.h"
#include "Metainfo\Signature.h"

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <assert.h>
#include <exception>
#include <vector>
#include <algorithm>

#ifdef HAVE_VISUAL_STUDIO
#include <intrin.h>
#include <windows.h>
#endif

#if HAVE_WINRT == 1
#include <future>
#endif

#ifdef _DEBUG
#define hAssert(expression) H::System::Assert(expression);
#else
#define hAssert(expression)
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

#if HAVE_WINRT == 1
		static HRESULT PerformSync(Windows::Foundation::IAsyncAction ^op);
		static void PerformSyncThrow(Windows::Foundation::IAsyncAction ^op);

		template<typename T>
		static HRESULT PerformSync(Windows::Foundation::IAsyncActionWithProgress<T> ^op) {
			std::promise<HRESULT> prom;
			auto fut = prom.get_future();

			op->Completed = ref new Windows::Foundation::AsyncActionWithProgressCompletedHandler<T>(
				[&](Windows::Foundation::IAsyncActionWithProgress<T> ^op, Windows::Foundation::AsyncStatus status) {
				HRESULT res;

				if (status == Windows::Foundation::AsyncStatus::Completed) {
					res = S_OK;
				}
				else {
					res = static_cast<HRESULT>(op->ErrorCode.Value);
				}

				prom.set_value(res);
			});

			auto result = fut.get();

			return result;
		}

		template<typename T>
		static void PerformSyncThrow(Windows::Foundation::IAsyncActionWithProgress<T> ^op) {
			HSystem::ThrowIfFailed(HSystem::PerformSync(op));
		}

		template<typename T>
		static std::pair<HRESULT, T> PerformSync(Windows::Foundation::IAsyncOperation<T> ^op) {
			std::promise<std::pair<HRESULT, T>> prom;	//break exception there
			auto fut = prom.get_future();

			op->Completed = ref new Windows::Foundation::AsyncOperationCompletedHandler<T>(
				[&](Windows::Foundation::IAsyncOperation<T> ^op, Windows::Foundation::AsyncStatus status) {
				std::pair<HRESULT, T> res;

				if (status == Windows::Foundation::AsyncStatus::Completed) {
					res.first = S_OK;
					res.second = op->GetResults();
				}
				else {
					res.first = static_cast<HRESULT>(op->ErrorCode.Value);
					res.second = T();
				}

				prom.set_value(res);
			});

			auto result = fut.get();

			return result;
		}

		template<typename T>
		static T PerformSyncThrow(Windows::Foundation::IAsyncOperation<T> ^op) {
			auto res = H::System::PerformSync(op);
			H::System::ThrowIfFailed(res.first);
			return res.second;
		}

		template<typename T, typename T2>
		static std::pair<HRESULT, T> PerformSync(Windows::Foundation::IAsyncOperationWithProgress<T, T2> ^op) {
			std::promise<std::pair<HRESULT, T>> prom;
			auto fut = prom.get_future();

			op->Completed = ref new Windows::Foundation::AsyncOperationWithProgressCompletedHandler<T, T2>(
				[&](Windows::Foundation::IAsyncOperationWithProgress<T, T2> ^op, Windows::Foundation::AsyncStatus status) {
				std::pair<HRESULT, T> res;

				if (status == Windows::Foundation::AsyncStatus::Completed) {
					res.first = S_OK;
					res.second = op->GetResults();
				}
				else {
					res.first = static_cast<HRESULT>(op->ErrorCode.Value);
					res.second = T();
				}

				prom.set_value(res);
			});

			auto result = fut.get();

			return result;
		}

		template<typename T, typename T2>
		static T PerformSyncThrow(Windows::Foundation::IAsyncOperationWithProgress<T, T2> ^op) {
			auto res = H::System::PerformSync(op);
			H::System::ThrowIfFailed(res.first);
			return res.second;
		}

		template<class T>
		static Windows::Foundation::TypedEventHandler<typename Signature<T>::A1, typename Signature<T>::A2> ^MakeTypedEventHandler(T fn) {
			auto res = ref new Windows::Foundation::TypedEventHandler<typename Signature<T>::A1, typename Signature<T>::A2>(fn);
			return res;
		}
#endif
	};
}