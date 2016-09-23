#pragma once

#include <windows.h>
#include <DirectXMath.h>
#include <cstdint>

struct GUIDHash {
	size_t operator()(const GUID &v) const;
};

struct XMUINT2Hash {
	size_t operator()(const DirectX::XMUINT2 &v) const;
};