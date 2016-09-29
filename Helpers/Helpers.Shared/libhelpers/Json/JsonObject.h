#pragma once

#include <cstdint>
#include <string>

class JsonObject {
public:
	JsonObject();
	virtual ~JsonObject();

	bool operator=(bool v);
	int operator=(int v);
	uint32_t operator=(uint32_t v);
	int64_t operator=(int64_t v);
	uint64_t operator=(uint64_t v);
	double operator=(double v);
	float operator=(float v);
	const std::string &operator=(const std::string &v);
	const std::wstring &operator=(const std::wstring &v);
	const char *operator=(const char *v);
	const wchar_t *operator=(const wchar_t *v);

	operator bool();
	operator int();
	operator uint32_t();
	operator int64_t();
	operator uint64_t();
	operator double();
	operator float();
	operator std::string();
	operator std::wstring();

	JsonObject &operator[](const char *name);
	JsonObject &operator[](int index);

	virtual std::string ToString() = 0;

protected:
	virtual void SetVal(bool v) = 0;
	virtual void SetVal(int v) = 0;
	virtual void SetVal(uint32_t v) = 0;
	virtual void SetVal(int64_t v) = 0;
	virtual void SetVal(uint64_t v) = 0;
	virtual void SetVal(double v) = 0;
	virtual void SetVal(float v) = 0;
	virtual void SetVal(const std::string &v) = 0;
	virtual void SetVal(const std::wstring &v) = 0;
	virtual void SetVal(const char *v) = 0;
	virtual void SetVal(const wchar_t *v) = 0;

	virtual void GetVal(bool &v) = 0;
	virtual void GetVal(int &v) = 0;
	virtual void GetVal(uint32_t &v) = 0;
	virtual void GetVal(int64_t &v) = 0;
	virtual void GetVal(uint64_t &v) = 0;
	virtual void GetVal(double &v) = 0;
	virtual void GetVal(float &v) = 0;
	virtual void GetVal(std::string &v) = 0;
	virtual void GetVal(std::wstring &v) = 0;

	virtual JsonObject &Get(const char *name) = 0;
	virtual JsonObject &Get(int index) = 0;
};