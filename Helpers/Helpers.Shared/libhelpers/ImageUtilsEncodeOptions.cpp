#include "ImageUtilsEncodeOptions.h"

ImageUtilsEncodeNoOptions::ImageUtilsEncodeNoOptions() {
}

HRESULT ImageUtilsEncodeNoOptions::SaveImpl(IPropertyBag2 *props) {
	return S_OK;
}

ImageUtilsEncodeJpegOptions::ImageUtilsEncodeJpegOptions()
	: Quality(1.0f) {
}

HRESULT ImageUtilsEncodeJpegOptions::SaveImpl(IPropertyBag2 *props) {
	PROPBAG2 option = { 0 };
	option.pstrName = L"ImageQuality";
	VARIANT varValue;
	varValue.vt = VT_R4;
	varValue.fltVal = this->Quality;

	return props->Write(1, &option, &varValue);
}