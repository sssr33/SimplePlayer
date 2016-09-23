#pragma once
#include "libhelpers\config.h"
#include "libhelpers\HMath.h"
#include "libhelpers\jsonNative\dist\json\json.h"

#include <DirectXMath.h>
#include <d2d1.h>
#include <dwrite.h>

namespace H {
	class Json {
	public:

		template<class T>
		static void Set(::Json::Value& obj, std::string name, T &&v) {
			obj[name] = ::Json::Value(Json::Serialize(v));
		}

		static ::Json::Value Serialize(bool v);
		static ::Json::Value Serialize(int v);
		static ::Json::Value Serialize(uint32_t v);
		static ::Json::Value Serialize(int64_t v);
		static ::Json::Value Serialize(uint64_t v);
		static ::Json::Value Serialize(double v);
		static ::Json::Value Serialize(float v);
		static ::Json::Value Serialize(const std::string &str);
		static ::Json::Value Serialize(const std::wstring &str);
		static ::Json::Value Serialize(const D2D1_RECT_F &v);
		static ::Json::Value Serialize(const DirectX::XMFLOAT3 &v);
		static ::Json::Value Serialize(const D2D1::ColorF &v);
		static ::Json::Value Serialize(const D2D1_COLOR_F &v);
		static ::Json::Value Serialize(const D2D1::Matrix3x2F &v);
		static ::Json::Value Serialize(const D2D1_MATRIX_3X2_F &v);
		static ::Json::Value Serialize(const D2D1_POINT_2F &v);
		static ::Json::Value Serialize(const DirectX::XMFLOAT2 &v);
		static ::Json::Value Serialize(const D2D1_BEZIER_SEGMENT &v);
		static ::Json::Value Serialize(const D2D1_STROKE_STYLE_PROPERTIES &v);
		static ::Json::Value Serialize(const DWRITE_TEXT_METRICS &v);
		static ::Json::Value Serialize(const D2D1_SIZE_F& obj);
		static ::Json::Value Serialize(const DirectX::XMFLOAT4 &v);
		static ::Json::Value Serialize(const DirectX::XMUINT2& v);
		static ::Json::Value Serialize(const DirectX::XMVECTOR& v);
		static ::Json::Value Serialize(const DirectX::XMUINT3& v);
		template<class T>
		static ::Json::Value Serialize(const std::vector<T>& v);


		template<class T>
		static void Get(const ::Json::Value& obj, std::string name, T &v) {
			Json::Deserialize(::Json::Value(obj[name]), v);
		}

		static bool Deserialize(const ::Json::Value& json, bool& out);
		static bool Deserialize(const ::Json::Value& json, int& out);
		static bool Deserialize(const ::Json::Value& json, uint32_t& out);
		static bool Deserialize(const ::Json::Value& json, int64_t& out);
		static bool Deserialize(const ::Json::Value& json, uint64_t& out);
		static bool Deserialize(const ::Json::Value& json, double& out);
		static bool Deserialize(const ::Json::Value& json, float& out);
		static bool Deserialize(const ::Json::Value& json, std::string& out);
		static bool Deserialize(const ::Json::Value& json, std::wstring& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_RECT_F& out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMFLOAT3& out);
		static bool Deserialize(const ::Json::Value& json, D2D1::ColorF& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_COLOR_F& out);
		static bool Deserialize(const ::Json::Value& json, D2D1::Matrix3x2F& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_MATRIX_3X2_F& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_POINT_2F& out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMFLOAT2& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_BEZIER_SEGMENT& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_STROKE_STYLE_PROPERTIES& out);
		static bool Deserialize(const ::Json::Value& json, DWRITE_TEXT_METRICS& out);
		static bool Deserialize(const ::Json::Value& json, D2D1_SIZE_F& out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMFLOAT4 & out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMUINT2& out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMVECTOR& out);
		static bool Deserialize(const ::Json::Value& json, DirectX::XMUINT3& out);

		template<class T>
		static bool Deserialize(const ::Json::Value& json, std::vector<T>& out);
	};

	template<class T>
	inline ::Json::Value Json::Serialize(const std::vector<T>& v) {
		::Json::Value res;

		for (auto& i : v) {
			res.append(Json::Value(Json::Serialize(i)));
		}

		return res;
	}

	template<class T>
	inline bool Json::Deserialize(const ::Json::Value& json, std::vector<T>& out) {
		bool res = true;
		if (json.isArray()) {
			for (auto& i : json) {
				T tmp;
				res = res && Json::Deserialize(i, tmp);
				out.push_back(tmp);
			}
			return res;
		}
		return false;
	}
}
