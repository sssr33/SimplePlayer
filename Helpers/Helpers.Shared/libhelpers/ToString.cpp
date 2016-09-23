#include "ToString.h"
#include "HMath.h"

std::string ToStringHelper<DirectX::XMFLOAT2>::Do(const DirectX::XMFLOAT2 &v) {
	std::string str =
		"{ " + std::to_string(v.x) + ", " + std::to_string(v.y) + "}";
	return str;
}

std::string ToStringHelper<DirectX::XMFLOAT3>::Do(const DirectX::XMFLOAT3 &v) {
	std::string str =
		"{ " + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + "}";
	return str;
}

std::string ToStringHelper<DirectX::XMVECTOR>::Do(const DirectX::XMVECTOR &v) {
	std::string str =
		"{ " + std::to_string(v.XF) + ", " + std::to_string(v.YF) + ", " + std::to_string(v.ZF) + ", " + std::to_string(v.WF) + "}";
	return str;
}

std::string ToStringHelper<DirectX::XMMATRIX>::Do(const DirectX::XMMATRIX &v) {
	std::string str =
		"{\n" + ToStr::Do(v.r[0]) + "\n" + ToStr::Do(v.r[1]) + "\n" + ToStr::Do(v.r[2]) + "\n" + ToStr::Do(v.r[3]) + "\n}";
	return str;
}

std::string ToStringHelper<D2D1::Matrix3x2F>::Do(const D2D1::Matrix3x2F &v) {
	std::string str = "{\n";
	str += ToStr::Do(DirectX::XMFLOAT2(v._11, v._12)) + "\n";
	str += ToStr::Do(DirectX::XMFLOAT2(v._21, v._22)) + "\n";
	str += ToStr::Do(DirectX::XMFLOAT2(v._31, v._32));
	str += "\n}";
	return str;
}