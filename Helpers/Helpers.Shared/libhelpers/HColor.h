#pragma once
#include "config.h"

#include <d2d1.h>
#include <DirectXMath.h>

namespace H {
	class Color {
	public:
		static DirectX::XMFLOAT4 PremultiplyColor(const DirectX::XMFLOAT4 &v);

		// WARNING: don't use if alpha can be 0
		static DirectX::XMFLOAT4 DemultiplyColor(const DirectX::XMFLOAT4 &v);

		template<class T> static T Transparent() {
			return ColorSetter<T>::Get(1.0f, 1.0f, 1.0f, 0.0f);
		}

		template<class T> static void Transparent(T &v) {
			v = ColorSetter<T>::Get(1.0f, 1.0f, 1.0f, 0.0f);
		}

	private:
		template<class T> struct ColorSetter {
			static T Get(float r, float g, float b, float a) {
				static_assert(false, "Not Implemented");
				return T();
			}
		};

		template<> struct ColorSetter<D2D1::ColorF> {
			static D2D1::ColorF Get(float r, float g, float b, float a) {
				return D2D1::ColorF(r, g, b, a);
			}
		};

		template<> struct ColorSetter<D2D1_COLOR_F> {
			static D2D1_COLOR_F Get(float r, float g, float b, float a) {
				return D2D1::ColorF(r, g, b, a);
			}
		};
	};
}