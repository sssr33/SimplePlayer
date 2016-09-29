#include "JsonObject.h"

JsonObject::JsonObject() {
}

JsonObject::~JsonObject() {
}

bool JsonObject::operator=(bool v) {
	this->SetVal(v);
	return v;
}

int JsonObject::operator=(int v) {
	this->SetVal(v);
	return v;
}

uint32_t JsonObject::operator=(uint32_t v) {
	this->SetVal(v);
	return v;
}

int64_t JsonObject::operator=(int64_t v) {
	this->SetVal(v);
	return v;
}

uint64_t JsonObject::operator=(uint64_t v) {
	this->SetVal(v);
	return v;
}

double JsonObject::operator=(double v) {
	this->SetVal(v);
	return v;
}

float JsonObject::operator=(float v) {
	this->SetVal(v);
	return v;
}

const std::string &JsonObject::operator=(const std::string &v) {
	this->SetVal(v);
	return v;
}

const std::wstring &JsonObject::operator=(const std::wstring &v) {
	this->SetVal(v);
	return v;
}

const char *JsonObject::operator=(const char *v) {
	this->SetVal(v);
	return v;
}

const wchar_t *JsonObject::operator=(const wchar_t *v) {
	this->SetVal(v);
	return v;
}

JsonObject::operator bool() {
	bool v;
	this->GetVal(v);
	return v;
}

JsonObject::operator int() {
	int v;
	this->GetVal(v);
	return v;
}

JsonObject::operator uint32_t() {
	uint32_t v;
	this->GetVal(v);
	return v;
}

JsonObject::operator int64_t() {
	int64_t v;
	this->GetVal(v);
	return v;
}

JsonObject::operator uint64_t() {
	uint64_t v;
	this->GetVal(v);
	return v;
}

JsonObject::operator double() {
	double v;
	this->GetVal(v);
	return v;
}

JsonObject::operator float() {
	float v;
	this->GetVal(v);
	return v;
}

JsonObject::operator std::string() {
	std::string v;
	this->GetVal(v);
	return v;
}

JsonObject::operator std::wstring() {
	std::wstring v;
	this->GetVal(v);
	return v;
}

JsonObject &JsonObject::operator[](const char *name) {
	return this->Get(name);
}

JsonObject &JsonObject::operator[](int index) {
	return this->Get(index);
}