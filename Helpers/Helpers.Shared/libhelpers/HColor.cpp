#include "HColor.h"

#include <exception>

namespace H {
	DirectX::XMFLOAT4 Color::PremultiplyColor(const DirectX::XMFLOAT4 &v) {
		DirectX::XMFLOAT4 premultiplied;

		premultiplied.x = v.x * v.w;
		premultiplied.y = v.y * v.w;
		premultiplied.z = v.z * v.w;
		premultiplied.w = v.w;

		return premultiplied;
	}

	DirectX::XMFLOAT4 Color::DemultiplyColor(const DirectX::XMFLOAT4 &v) {
		DirectX::XMFLOAT4 demultiplied;

		if (v.w == 0.0f) {
			throw std::exception("Can't demultiply correctly with 0 alpha");
		}

		float tmp = 1.0f / v.w;

		demultiplied.x = v.x * tmp;
		demultiplied.y = v.y * tmp;
		demultiplied.z = v.z * tmp;
		demultiplied.w = v.w;

		return demultiplied;
	}
}