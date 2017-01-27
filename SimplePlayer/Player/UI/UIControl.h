#pragma once

namespace UI {
	class Control {
	public:
		Control();
		virtual ~Control();

		virtual float X() = 0;
		virtual void X(float v) = 0;

		virtual float Y() = 0;
		virtual void Y(float v) = 0;

		virtual float SizeX() = 0;
		virtual void SizeX(float v) = 0;

		virtual float SizeY() = 0;
		virtual void SizeY(float v) = 0;
	};
}