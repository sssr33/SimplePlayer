#include "HJson.h"

#include <locale>
#include <codecvt>

namespace H {
	::Json::Value Json::Serialize(bool v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(int v) {
		::Json::Value obj;
		obj["int"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(uint32_t v) {
		::Json::Value obj;
		obj["uint32_t"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(int64_t v) {
		::Json::Value obj;
		obj["int64_t"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(uint64_t v) {
		::Json::Value obj;
		obj["uint64_t"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(double v) {
		::Json::Value obj;
		obj["double"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(float v) {
		::Json::Value obj;
		obj["float"] = v;
		return obj;
	}

	::Json::Value Json::Serialize(const std::string & str) {
		::Json::Value obj;
		obj["string"] = str;
		return obj;
	}

	::Json::Value Json::Serialize(const std::wstring & str) {
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(str);


		return Json::Serialize(converted_str);
	}

	::Json::Value Json::Serialize(const D2D1_RECT_F & v) {
		::Json::Value res;
		res["bottom"] = v.bottom;
		res["left"] = v.left;
		res["right"] = v.right;
		res["top"] = v.top;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMFLOAT3 & v) {
		::Json::Value res;
		res["x"] = v.x;
		res["y"] = v.y;
		res["z"] = v.z;
		return res;
	}

	::Json::Value Json::Serialize(const D2D1::ColorF & v) {
		::Json::Value res;
		res["a"] = v.a;
		res["r"] = v.r;
		res["g"] = v.g;
		res["b"] = v.b;
		return res;
	}

	::Json::Value Json::Serialize(const D2D1_COLOR_F & v) {
		::Json::Value res;
		res["a"] = v.a;
		res["r"] = v.r;
		res["g"] = v.g;
		res["b"] = v.b;
		return res;
	}

	::Json::Value Json::Serialize(const D2D1::Matrix3x2F & v) {
		return Json::Serialize((const D2D1_MATRIX_3X2_F &)v);
	}

	::Json::Value Json::Serialize(const D2D1_MATRIX_3X2_F & v) {
		::Json::Value res;
		res["_11"] = v._11;
		res["_12"] = v._12;
		res["_21"] = v._21;
		res["_22"] = v._22;
		res["_31"] = v._31;
		res["_32"] = v._32;
		return res;
	}

	::Json::Value Json::Serialize(const D2D1_POINT_2F & v) {
		::Json::Value res;
		res["x"] = v.x;
		res["y"] = v.y;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMFLOAT2 & v) {
		return Json::Serialize(D2D1::Point2F(v.x, v.y));
	}

	::Json::Value Json::Serialize(const D2D1_BEZIER_SEGMENT & v) {
		::Json::Value res;
		res["point1"] = Json::Serialize(v.point1);
		res["point2"] = Json::Serialize(v.point2);
		res["point3"] = Json::Serialize(v.point3);
		return res;
	}

	::Json::Value Json::Serialize(const D2D1_STROKE_STYLE_PROPERTIES & v) {
		::Json::Value res;
		res["startCap"] = (int)v.startCap;
		res["endCap"] = (int)v.endCap;
		res["dashCap"] = (int)v.dashCap;
		res["lineJoin"] = (int)v.lineJoin;
		res["miterLimit"] = v.miterLimit;
		res["dashStyle"] = (int)v.dashStyle;
		res["dashOffset"] = v.dashOffset;

		return res;
	}

	::Json::Value Json::Serialize(const DWRITE_TEXT_METRICS & v) {
		::Json::Value res;

		res["left"] = v.left;
		res["top"] = v.top;
		res["width"] = v.width;
		res["height"] = v.height;
		res["widthIncludingTrailingWhitespace"] = v.widthIncludingTrailingWhitespace;
		res["layoutWidth"] = v.layoutWidth;
		res["layoutHeight"] = v.layoutHeight;
		res["maxBidiReorderingDepth"] = v.maxBidiReorderingDepth;
		res["lineCount"] = v.lineCount;

		return res;
	}

	::Json::Value Json::Serialize(const D2D1_SIZE_F & obj) {
		::Json::Value res;
		res["w"] = obj.width;
		res["h"] = obj.height;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMFLOAT4 & v) {
		::Json::Value res;
		res["x"] = v.x;
		res["y"] = v.y;
		res["z"] = v.z;
		res["w"] = v.w;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMUINT2 & v) {
		::Json::Value res;
		res["x"] = v.x;
		res["y"] = v.y;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMVECTOR & v) {
		::Json::Value res;
		res["XF"] = v.XF;
		res["YF"] = v.YF;
		res["ZF"] = v.ZF;
		res["WF"] = v.WF;
		return res;
	}

	::Json::Value Json::Serialize(const DirectX::XMUINT3 & v) {
		::Json::Value res;
		res["x"] = v.x;
		res["y"] = v.y;
		res["z"] = v.z;
		return res;
	}

	bool Json::Deserialize(const ::Json::Value& json, bool & out) {
		if (json.isBool()) {
			out = json.asBool();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, int & out) {
		if (json["int"].isInt()) {
			out = json["int"].asInt();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, uint32_t & out) {
		if (json["uint32_t"].isUInt()) {
			out = json["uint32_t"].asUInt();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, int64_t & out) {
		if (json["int64_t"].isInt64()) {
			out = json["int64_t"].asInt64();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, uint64_t & out) {
		if (json["uint64_t"].isUInt64()) {
			out = json["uint64_t"].asUInt64();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, double & out) {
		if (json["double"].isDouble()) {
			out = json["double"].asDouble();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, float & out) {
		if (!json["float"].isNull()) {
			out = json["float"].asFloat();
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, std::string & out) {
		if (json["string"].isString()) {
			out = json["string"].asString();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, std::wstring & out) {
		if (!json["string"].isNull()) {
			std::string tmp;
			tmp = json["string"].asString();
			out = std::wstring(tmp.begin(), tmp.end());
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_RECT_F & out) {
		bool res = json["left"].isNull() && json["top"].isNull() && json["right"].isNull() && json["bottom"].isNull();
		out.bottom = json["bottom"].asFloat();
		out.left = json["left"].asFloat();
		out.right = json["right"].asFloat();
		out.top = json["top"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT3 & out) {
		bool res = json["x"].isNull() && json["y"].isNull() && json["z"].isNull();
		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		out.z = json["z"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1::ColorF & out) {
		bool res = json["a"].isNull() && json["r"].isNull() && json["g"].isNull() && json["b"].isNull();
		out.a = json["a"].asFloat();
		out.r = json["r"].asFloat();
		out.g = json["g"].asFloat();
		out.b = json["b"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_COLOR_F & out) {
		bool res = json["a"].isNull() && json["r"].isNull() && json["g"].isNull() && json["b"].isNull();
		out.a = json["a"].asFloat();
		out.r = json["r"].asFloat();
		out.g = json["g"].asFloat();
		out.b = json["b"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1::Matrix3x2F & out) {
		bool res = json["_11"].isNull() && json["_12"].isNull() && json["_21"].isNull() && json["_22"].isNull() && json["_31"].isNull() && json["_32"].isNull();
		out._11 = json["_11"].asFloat();
		out._12 = json["_12"].asFloat();
		out._21 = json["_21"].asFloat();
		out._22 = json["_22"].asFloat();
		out._31 = json["_31"].asFloat();
		out._32 = json["_32"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_MATRIX_3X2_F & out) {
		bool res = json["_11"].isNull() && json["_12"].isNull() && json["_21"].isNull() && json["_22"].isNull() && json["_31"].isNull() && json["_32"].isNull();
		out._11 = json["_11"].asFloat();
		out._12 = json["_12"].asFloat();
		out._21 = json["_21"].asFloat();
		out._22 = json["_22"].asFloat();
		out._31 = json["_31"].asFloat();
		out._32 = json["_32"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_POINT_2F & out) {
		bool res = json["x"].isNull() && json["y"].isNull();
		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT2 & out) {
		bool res = json["x"].isNull() && json["y"].isNull();
		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_BEZIER_SEGMENT & out) {
		bool res = json["point1"].isNull() && json["point2"].isNull() && json["point3"].isNull();
		res = res && Json::Deserialize(json["point1"], out.point1);
		res = res && Json::Deserialize(json["point2"], out.point2);
		res = res && Json::Deserialize(json["point3"], out.point3);
		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_STROKE_STYLE_PROPERTIES & out) {
		bool res = json["startCap"].isNull() && json["endCap"].isNull() && json["dashCap"].isNull() &&
			json["lineJoin"].isNull() && json["miterLimit"].isNull() && json["dashStyle"].isNull() && json["dashOffset"].isNull();

		out.dashCap = (D2D1_CAP_STYLE)json["dashCap"].asInt();
		out.dashOffset = json["dashOffset"].asFloat();
		out.dashStyle = (D2D1_DASH_STYLE)json["dashStyle"].asInt();
		out.endCap = (D2D1_CAP_STYLE)json["endCap"].asInt();
		out.lineJoin = (D2D1_LINE_JOIN)json["lineJoin"].asInt();
		out.miterLimit = json["miterLimit"].asFloat();
		out.startCap = (D2D1_CAP_STYLE)json["startCap"].asInt();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DWRITE_TEXT_METRICS & out) {
		bool res = json["left"].isNull() && json["top"].isNull() && json["width"].isNull() && json["widthIncludingTrailingWhitespace"].isNull() &&
			json["layoutWidth"].isNull() && json["layoutHeight"].isNull() && json["maxBidiReorderingDepth"].isNull() && json["lineCount"].isNull() &&
			json["height"].isNull();

		out.height = json["height"].asFloat();
		out.layoutHeight = json["layoutHeight"].asFloat();
		out.layoutWidth = json["layoutWidth"].asFloat();
		out.left = json["left"].asFloat();
		out.lineCount = json["lineCount"].asUInt();
		out.maxBidiReorderingDepth = json["maxBidiReorderingDepth"].asUInt();
		out.top = json["top"].asFloat();
		out.width = json["width"].asFloat();
		out.widthIncludingTrailingWhitespace = json["widthIncludingTrailingWhitespace"].asFloat();

		return !res;
	}

#if HAVE_WINRT == 1
	bool Json::Deserialize(const ::Json::Value& json, Windows::Foundation::Size & out) {
		bool res = json["W"].isNull() && json["H"].isNull();

		out.Width = json["W"].asFloat();
		out.Height = json["H"].asFloat();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, Windows::Foundation::Point & out) {
		bool res = json["X"].isNull() && json["Y"].isNull();

		out.X = json["X"].asFloat();
		out.Y = json["Y"].asFloat();

		return !res;
	}
#endif

	bool Json::Deserialize(const ::Json::Value& json, D2D1_SIZE_F & out) {
		bool res = json["w"].isNull() && json["h"].isNull();

		out.height = json["h"].asFloat();
		out.width = json["w"].asFloat();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT4 & out) {
		bool res = json["x"].isNull() && json["y"].isNull() && json["z"].isNull() && json["w"].isNull();

		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		out.z = json["z"].asFloat();
		out.w = json["w"].asFloat();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMUINT2 & out) {
		bool res = json["x"].isNull() && json["y"].isNull();

		out.x = json["x"].asUInt();
		out.y = json["y"].asUInt();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMVECTOR & out) {
		bool res = json["XF"].isNull() && json["YF"].isNull() && json["ZF"].isNull() && json["WF"].isNull();

		out.XF = json["XF"].asFloat();
		out.YF = json["YF"].asFloat();
		out.ZF = json["ZF"].asFloat();
		out.WF = json["WF"].asFloat();

		return !res;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMUINT3 & out) {
		bool res = json["x"].isNull() && json["y"].isNull() && json["z"].isNull();

		out.x = json["x"].asUInt();
		out.y = json["y"].asUInt();
		out.z = json["z"].asUInt();

		return !res;
	}
}