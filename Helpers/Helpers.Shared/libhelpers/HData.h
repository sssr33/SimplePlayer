#pragma once
#include "config.h"
#include "TypeConverter.h"
#include "Meta\ZeroDefaultValue.h"
#include "raw_ptr.h"

#ifdef HAVE_VISUAL_STUDIO
#include <intrin.h>
#endif

#include <memory>
#include <cstdint>
#include <array>

namespace H {
	class Data {
	public:
		// source http://stackoverflow.com/a/25808559/2440195
		static size_t CountSetBits(uint8_t v);
		static size_t CountSetBits(uint16_t v);
		static size_t CountSetBits(uint32_t v);
		static size_t CountSetBits(uint64_t v);
		static size_t CountSetBits(int8_t v);
		static size_t CountSetBits(int16_t v);
		static size_t CountSetBits(int32_t v);
		static size_t CountSetBits(int64_t v);
		static size_t CountSetBits(const void *data, size_t size);

		static void HashRound(uint32_t &hash, uint32_t v);

		static size_t CombineHash(size_t a, size_t b);

		template<int intBits>
		static int ToFixedPoint(float v) {
			const int fractBits = (sizeof(int) * 8) - intBits;
			const float scale = static_cast<float>(1 << fractBits);

			int res = static_cast<int>(v * scale);
			return res;
		}

		template<int intBits>
		static float FromFixedPoint(int v) {
			const int fractBits = (sizeof(int) * 8) - intBits;
			const float scale = 1.0f / static_cast<float>(1 << fractBits);

			float res = static_cast<float>(v) * scale;
			return res;
		}

		template<class T>
		static T SwapBytes(const T &v) {
			T dst;
			const uint8_t *srcPtr = reinterpret_cast<const uint8_t *>(&v);
			uint8_t *dstPtr = reinterpret_cast<uint8_t *>(&dst);

			for (size_t i = 0; i < sizeof(T); i++) {
				dstPtr[i] = srcPtr[sizeof(T) - i - 1];
			}

			return dst;
		}

		template<>
		static uint64_t SwapBytes(const uint64_t &v) {
#if HAVE_VISUAL_STUDIO == 1
			return _byteswap_uint64(v);
#else
			uint64_t dst = (v & 0xFF) << 56 | (v >> 8 & 0xFF) << 48 |
				(v >> 16 & 0xFF) << 40 | (v >> 24 & 0xFF) << 32 |
				(v >> 32 & 0xFF) << 24 | (v >> 40 & 0xFF) << 16 |
				(v >> 48 & 0xFF) << 8 | (v >> 56 & 0xFF);
			return dst;
#endif
		}

		template<>
		static int64_t SwapBytes(const int64_t &v) {
			return SwapBytes(static_cast<const uint64_t &>(v));
		}

		template<>
		static uint32_t SwapBytes(const uint32_t &v) {
#if HAVE_VISUAL_STUDIO == 1
			return _byteswap_ulong(v);
#else
			uint32_t dst = (v & 0xFF) << 24 | (v >> 8 & 0xFF) << 16 |
				(v >> 16 & 0xFF) << 8 | (v >> 24 & 0xFF);
			return dst;
#endif
		}

		template<>
		static int32_t SwapBytes(const int32_t &v) {
			return SwapBytes(static_cast<const uint32_t &>(v));
		}

		template<>
		static uint16_t SwapBytes(const uint16_t &v) {
#if HAVE_VISUAL_STUDIO == 1
			return _byteswap_ushort(v);
#else
			uint16_t dst = (v & 0xFF) << 8 | (v >> 8 & 0xFF);
			return dst;
#endif
		}

		template<>
		static int16_t SwapBytes(const int16_t &v) {
			return SwapBytes(static_cast<const uint16_t &>(v));
		}

		template<>
		static uint8_t SwapBytes(const uint8_t &v) {
			return v;
		}

		template<>
		static int8_t SwapBytes(const int8_t &v) {
			return v;
		}

		template<class D, class S>
		static D Convert(const S &v) {
			return TypeConverter<D, S>::Convert(v);
		}

		template<template<class> class DefaultValueType = ZeroDefaultValue, class PtrType, class T, class R>
		static R InstanceOrDefaultValue(PtrType &obj, R(T::*getter)()) {
			if (obj) {
				return ((*obj).*getter)();
			}
			else {
				return DefaultValueType<R>::Get();
			}
		}

		template<template<class> class DefaultValueType = ZeroDefaultValue, class PtrType, class T, class R>
		static R InstanceOrDefaultValue(PtrType const &obj, R(T::*getter)() const) {
			if (obj) {
				return ((*obj).*getter)();
			}
			else {
				return DefaultValueType<R>::Get();
			}
		}

		template<class T, template<class> class DefaultValueType = ZeroDefaultValue>
		static T DefaultValue() {
			return DefaultValueType<T>::Get();
		}
	};
}