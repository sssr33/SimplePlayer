#pragma once
#include <stdexcept>
#include <Windows.h>

class HResultException : public std::runtime_error {
public:
	HResultException(HRESULT hr);

	HRESULT GetHRESULT() const;

private:
	HRESULT hr;
};