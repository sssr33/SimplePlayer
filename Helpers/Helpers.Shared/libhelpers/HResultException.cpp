#include "HResultException.h"

#include <sstream>

HResultException::HResultException(HRESULT hr) 
	: std::runtime_error("HResultException"), hr(hr)
{
}

HRESULT HResultException::GetHRESULT() const {
	return this->hr;
}