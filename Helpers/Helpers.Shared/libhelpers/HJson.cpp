#include "HJson.h"

#include <locale>
#include <codecvt>

namespace H {
	::Json::Value Json::Serialize(bool v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(int v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(uint32_t v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(int64_t v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(uint64_t v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(double v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(float v) {
		::Json::Value obj = v;
		return obj;
	}

	::Json::Value Json::Serialize(const std::string & str) {
		::Json::Value obj = str;
		return obj;
	}

	::Json::Value Json::Serialize(const std::wstring & str) {
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(str);

		return Json::Serialize(converted_str);
	}

	::Json::Value Json::Serialize(const char *str) {
		::Json::Value obj = str;
		return obj;
	}

	::Json::Value Json::Serialize(const wchar_t *str) {
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
		res["x"] = v.XF;
		res["y"] = v.YF;
		res["z"] = v.ZF;
		res["w"] = v.WF;
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
		if (json.isInt()) {
			out = json.asInt();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, uint32_t & out) {
		if (json.isUInt()) {
			out = json.asUInt();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, int64_t & out) {
		if (json.isInt64()) {
			out = json.asInt64();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, uint64_t & out) {
		if (json.isUInt64()) {
			out = json.asUInt64();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, double & out) {
		if (json.isDouble()) {
			out = json.asDouble();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, float & out) {
		if (json.isDouble()) {
			out = json.asFloat();
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, std::string & out) {
		if (json.isString()) {
			out = json.asString();
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, std::wstring & out) {
		if (json.isString()) {
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string tmp;
			tmp = json.asString();
			out = converter.from_bytes(tmp);
			return true;
		}
		return false;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_RECT_F & out) {
		if (!json["left"].isDouble() || !json["top"].isDouble() || 
			!json["right"].isDouble() || !json["bottom"].isDouble()) 
		{
			return false;
		}

		out.bottom = json["bottom"].asFloat();
		out.left = json["left"].asFloat();
		out.right = json["right"].asFloat();
		out.top = json["top"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT3 & out) {
		if (!json["x"].isDouble() || !json["y"].isDouble() || !json["z"].isDouble()) {
			return false;
		}

		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		out.z = json["z"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1::ColorF & out) {
		if (!json["a"].isDouble() || !json["r"].isDouble() || !json["g"].isDouble() ||
			!json["b"].isDouble())
		{
			return false;
		}

		out.a = json["a"].asFloat();
		out.r = json["r"].asFloat();
		out.g = json["g"].asFloat();
		out.b = json["b"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_COLOR_F & out) {
		if (!json["a"].isDouble() || !json["r"].isDouble() || !json["g"].isDouble() || 
			!json["b"].isDouble()) 
		{
			return false;
		}

		out.a = json["a"].asFloat();
		out.r = json["r"].asFloat();
		out.g = json["g"].asFloat();
		out.b = json["b"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1::Matrix3x2F & out) {
		if (!json["_11"].isDouble() || !json["_12"].isDouble() || !json["_21"].isDouble() ||
			!json["_22"].isDouble() || !json["_31"].isDouble() | !json["_32"].isDouble())
		{
			return false;
		}

		out._11 = json["_11"].asFloat();
		out._12 = json["_12"].asFloat();
		out._21 = json["_21"].asFloat();
		out._22 = json["_22"].asFloat();
		out._31 = json["_31"].asFloat();
		out._32 = json["_32"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_MATRIX_3X2_F & out) {
		if (!json["_11"].isDouble() || !json["_12"].isDouble() || !json["_21"].isDouble() || 
			!json["_22"].isDouble() || !json["_31"].isDouble() | !json["_32"].isDouble()) 
		{
			return false;
		}

		out._11 = json["_11"].asFloat();
		out._12 = json["_12"].asFloat();
		out._21 = json["_21"].asFloat();
		out._22 = json["_22"].asFloat();
		out._31 = json["_31"].asFloat();
		out._32 = json["_32"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_POINT_2F & out) {
		if (!json["x"].isDouble() || !json["y"].isDouble()) {
			return false;
		}

		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT2 & out) {
		if (!json["x"].isDouble() || !json["y"].isDouble()) {
			return false;
		}

		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_BEZIER_SEGMENT & out) {
		if (!Json::Deserialize(json["point1"], out.point1)) {
			return false;
		}

		if (!Json::Deserialize(json["point2"], out.point2)) {
			return false;
		}

		if (!Json::Deserialize(json["point3"], out.point3)) {
			return false;
		}

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_STROKE_STYLE_PROPERTIES & out) {
		if (!json["dashCap"].isInt() || !json["dashOffset"].isDouble() || !json["dashStyle"].isInt() ||
			!json["endCap"].isInt() || !json["lineJoin"].isInt() || !json["miterLimit"].isDouble() ||
			!json["startCap"].isInt())
		{
			return false;
		}

		out.dashCap = (D2D1_CAP_STYLE)json["dashCap"].asInt();
		out.dashOffset = json["dashOffset"].asFloat();
		out.dashStyle = (D2D1_DASH_STYLE)json["dashStyle"].asInt();
		out.endCap = (D2D1_CAP_STYLE)json["endCap"].asInt();
		out.lineJoin = (D2D1_LINE_JOIN)json["lineJoin"].asInt();
		out.miterLimit = json["miterLimit"].asFloat();
		out.startCap = (D2D1_CAP_STYLE)json["startCap"].asInt();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DWRITE_TEXT_METRICS & out) {
		if (!json["left"].isDouble() || !json["top"].isDouble() || !json["width"].isDouble() || !json["height"].isDouble() ||
			!json["layoutWidth"].isDouble() || !json["layoutHeight"].isDouble() || !json["lineCount"].isUInt() ||
			!json["maxBidiReorderingDepth"].isUInt() || json["widthIncludingTrailingWhitespace"].isDouble())
		{
			return false;
		}

		out.left = json["left"].asFloat();
		out.top = json["top"].asFloat();
		out.width = json["width"].asFloat();
		out.height = json["height"].asFloat();
		out.layoutWidth = json["layoutWidth"].asFloat();
		out.layoutHeight = json["layoutHeight"].asFloat();
		out.lineCount = json["lineCount"].asUInt();
		out.maxBidiReorderingDepth = json["maxBidiReorderingDepth"].asUInt();
		out.widthIncludingTrailingWhitespace = json["widthIncludingTrailingWhitespace"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, D2D1_SIZE_F & out) {
		if (!json["w"].isDouble() || !json["h"].isDouble()) {
			return false;
		}

		out.width = json["w"].asFloat();
		out.height = json["h"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMFLOAT4 & out) {
		if (!json["x"].isDouble() || !json["y"].isDouble() || !json["z"].isDouble() || !json["w"].isDouble()) {
			return false;
		}

		out.x = json["x"].asFloat();
		out.y = json["y"].asFloat();
		out.z = json["z"].asFloat();
		out.w = json["w"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMUINT2 & out) {
		if (!json["x"].isUInt() || !json["y"].isUInt()) {
			return false;
		}

		out.x = json["x"].asUInt();
		out.y = json["y"].asUInt();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMVECTOR & out) {
		if (!json["x"].isDouble() || !json["y"].isDouble() || !json["z"].isDouble() || !json["w"].isDouble()) {
			return false;
		}

		out.XF = json["x"].asFloat();
		out.YF = json["y"].asFloat();
		out.ZF = json["z"].asFloat();
		out.WF = json["w"].asFloat();

		return true;
	}

	bool Json::Deserialize(const ::Json::Value& json, DirectX::XMUINT3 & out) {
		if (!json["x"].isUInt() || !json["y"].isUInt() || !json["z"].isUInt()) {
			return false;
		}

		out.x = json["x"].asUInt();
		out.y = json["y"].asUInt();
		out.z = json["z"].asUInt();

		return true;
	}
}