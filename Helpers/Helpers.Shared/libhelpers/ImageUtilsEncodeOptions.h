#pragma once

#include <wincodec.h>

template<class Impl>
class ImageUtilsEncodeOptions {
public:
	HRESULT Save(IPropertyBag2 *props) {
		return this->SaveImpl(props);
	}
protected:
	ImageUtilsEncodeOptions() {}
	~ImageUtilsEncodeOptions() {}

	HRESULT SaveImpl(IPropertyBag2 *props) {
		return static_cast<Impl *>(this)->SaveImpl(props);
	}
};

class ImageUtilsEncodeNoOptions : public ImageUtilsEncodeOptions<ImageUtilsEncodeNoOptions> {
public:
	ImageUtilsEncodeNoOptions();
	HRESULT SaveImpl(IPropertyBag2 *props);
};

class ImageUtilsEncodeJpegOptions : public ImageUtilsEncodeOptions<ImageUtilsEncodeJpegOptions> {
public:
	float Quality;

	ImageUtilsEncodeJpegOptions();
	HRESULT SaveImpl(IPropertyBag2 *props);
};