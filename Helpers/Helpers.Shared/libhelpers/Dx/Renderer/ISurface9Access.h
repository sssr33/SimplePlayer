#pragma once

struct Surface9Data {
	void *surface;
	int width;
	int height;
};

class ISurface9Access {
public:
	ISurface9Access() {}
	virtual ~ISurface9Access() {}

	virtual void Resize(int width, int height) = 0;
	virtual Surface9Data NextSurface() = 0;
};