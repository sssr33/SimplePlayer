#pragma once
#include "JsonObject.h"
#include "libhelpers\jsonNative\dist\json\json.h"

#include <memory>

class JsonObjectNativeRef : public JsonObject {
public:
	JsonObjectNativeRef(::Json::Value &obj);
	virtual ~JsonObjectNativeRef();

	std::string ToString() override;

protected:
	::Json::Value &obj;
	std::unique_ptr<JsonObjectNativeRef> ref;

	void SetVal(bool v) override;
	void SetVal(int v) override;
	void SetVal(uint32_t v) override;
	void SetVal(int64_t v) override;
	void SetVal(uint64_t v) override;
	void SetVal(double v) override;
	void SetVal(float v) override;
	void SetVal(const std::string &v) override;
	void SetVal(const std::wstring &v) override;
	void SetVal(const char *v) override;
	void SetVal(const wchar_t *v) override;

	void GetVal(bool &v) override;
	void GetVal(int &v) override;
	void GetVal(uint32_t &v) override;
	void GetVal(int64_t &v) override;
	void GetVal(uint64_t &v) override;
	void GetVal(double &v) override;
	void GetVal(float &v) override;
	void GetVal(std::string &v) override;
	void GetVal(std::wstring &v) override;

	JsonObject &Get(const char *name) override;
	JsonObject &Get(int index) override;
};

class JsonObjectNative : public JsonObjectNativeRef {
public:
	JsonObjectNative();
	virtual ~JsonObjectNative();

protected:
	::Json::Value obj;
};