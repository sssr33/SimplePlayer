#include "JsonObjectNative.h"

#include <codecvt>

JsonObjectNativeRef::JsonObjectNativeRef(::Json::Value &obj) 
	: obj(obj)
{
}

JsonObjectNativeRef::~JsonObjectNativeRef() {
}

std::string JsonObjectNativeRef::ToString() {
	Json::FastWriter writer;
	std::string res = writer.write(this->obj);
	return res;
}

void JsonObjectNativeRef::SetVal(bool v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(int v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(uint32_t v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(int64_t v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(uint64_t v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(double v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(float v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(const std::string &v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(const std::wstring &v) {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_str = converter.to_bytes(v);

	this->obj = converted_str;
}

void JsonObjectNativeRef::SetVal(const char *v) {
	this->obj = v;
}

void JsonObjectNativeRef::SetVal(const wchar_t *v) {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_str = converter.to_bytes(v);

	this->obj = converted_str;
}

void JsonObjectNativeRef::GetVal(bool &v) {
	v = this->obj.asBool();
}

void JsonObjectNativeRef::GetVal(int &v) {
	v = this->obj.asInt();
}

void JsonObjectNativeRef::GetVal(uint32_t &v) {
	v = this->obj.asUInt();
}

void JsonObjectNativeRef::GetVal(int64_t &v) {
	v = this->obj.asInt64();
}

void JsonObjectNativeRef::GetVal(uint64_t &v) {
	v = this->obj.asUInt64();
}

void JsonObjectNativeRef::GetVal(double &v) {
	v = this->obj.asDouble();
}

void JsonObjectNativeRef::GetVal(float &v) {
	v = this->obj.asFloat();
}

void JsonObjectNativeRef::GetVal(std::string &v) {
	v = this->obj.asString();
}

void JsonObjectNativeRef::GetVal(std::wstring &v) {
	auto tmp = this->obj.asString();
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	v = converter.from_bytes(tmp);
}

JsonObject &JsonObjectNativeRef::Get(const char *name) {
	this->ref = std::make_unique<JsonObjectNativeRef>(this->obj[name]);
	return *this->ref;
}

JsonObject &JsonObjectNativeRef::Get(int index) {
	this->ref = std::make_unique<JsonObjectNativeRef>(this->obj[index]);
	return *this->ref;
}

JsonObjectNative::JsonObjectNative() 
	: JsonObjectNativeRef(this->obj)
{
}

JsonObjectNative::~JsonObjectNative() {
}