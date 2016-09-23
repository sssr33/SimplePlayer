#pragma once

#include <d2d1.h>
#include <DirectXMath.h>
#include <string>

template<class T> struct ToStringHelper {
	static std::string Do(const T &v) {
		static_assert(false, "Not implemented");
	}
};

struct ToStr {
	template<class T> static std::string Do(const T &v) {
		return ToStringHelper<T>::Do(v);
	}
};

template<> struct ToStringHelper<DirectX::XMFLOAT2> {
	static std::string Do(const DirectX::XMFLOAT2 &v);
};

template<> struct ToStringHelper<DirectX::XMFLOAT3> {
	static std::string Do(const DirectX::XMFLOAT3 &v);
};

template<> struct ToStringHelper<DirectX::XMVECTOR>{
	static std::string Do(const DirectX::XMVECTOR &v);
};

template<> struct ToStringHelper<DirectX::XMMATRIX> {
	static std::string Do(const DirectX::XMMATRIX &v);
};

template<> struct ToStringHelper<D2D1::Matrix3x2F> {
	static std::string Do(const D2D1::Matrix3x2F &v);
};