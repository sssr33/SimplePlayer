// SimplePlayer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <libhelpers\HJson.h>

/*
TODO
- Json as object(interface + object)
- exceptions instead of true/false (maybe)
- 2 versions of get
- remove unnecessary json fields on elementary types
- serialize/deserialize as protected or private
- try subscript operator + accessor insted of get/set methods
*/

struct AAcc {
	std::string str = "Hello";
	int i = 123;

	AAcc &operator=(int a) {
		this->i = a;
		return *this;
	}

	AAcc &operator=(const std::string &a) {
		this->str = a;
		return *this;
	}

	operator int() { 
		return this->i; 
	}

	operator std::string() {
		return this->str;
	}
};

struct A {
	AAcc aac;

	AAcc &operator[](const int index) {
		return this->aac;
	}
};

void test(int a, const std::string &s) {
	int stop = 324;
}

int main() {
	Json::Value json;

	H::Json::Set(json, "BoolVal", true);
	H::Json::Set(json, "IntVal", 1);
	H::Json::Set(json, "DblVal", 3.14);
	H::Json::Set(json, "FltVal", 3.14f);
	H::Json::Set(json, "StrVal", L"3,14");
	H::Json::Set(json, "U64Val", UINT64_MAX);

	bool b = false;
	int i = 0;
	double d;
	float f;
	std::wstring ws;
	uint64_t u64;

	H::Json::Get(json, "BoolVal", b);
	H::Json::Get(json, "IntVal", i);
	H::Json::Get(json, "IntVal", d);
	H::Json::Get(json, "FltVal", f);
	H::Json::Get(json, "StrVal", ws);
	H::Json::Get(json, "U64Val", u64);

	Json::FastWriter writer;
	std::string res = writer.write(json);

	A a;

	a[123] = 23;

	int asd = a[123];

	a[23] = "Hello world";

	std::string s = a[23];

	test(a[123], a[123]);

    return 0;
}

