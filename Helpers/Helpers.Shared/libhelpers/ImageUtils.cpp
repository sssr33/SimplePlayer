#include "ImageUtils.h"
#include "HSystem.h"

ImageUtilsStaticData::ImageUtilsStaticData() {
	// TODO: add bit sizes for all formats
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppPBGRA, 32));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppPRGBA, 32));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppBGRA, 32));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat32bppRGBA, 32));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat24bppBGR, 24));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat24bppRGB, 24));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat1bppIndexed, 1));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat2bppIndexed, 2));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat4bppIndexed, 4));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppIndexed, 8));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormatBlackWhite, 1));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat2bppGray, 2));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat4bppGray, 4));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppGray, 8));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat8bppAlpha, 8));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGR555, 16));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGR565, 16));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppBGRA5551, 16));
	this->WICPixelFormatBitSize.insert(std::make_pair(GUID_WICPixelFormat16bppGray, 16));
}

ImageUtilsStaticData *ImageUtils::StaticData() {
	static ImageUtilsStaticData data;
	return &data;
}

ImageUtils::ImageUtils() {
	H::System::ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(this->wicFactory.ReleaseAndGetAddressOf())
		)
	);
}

ImageUtils::~ImageUtils() {
}

uint32_t ImageUtils::GetFrameCount(IWICBitmapDecoder *decoder) const {
	HRESULT hr = S_OK;
	uint32_t count = 0;

	hr = decoder->GetFrameCount(&count);
	H::System::ThrowIfFailed(hr);

	return count;
}

uint32_t ImageUtils::GetPixelBitSize(IWICBitmapSource *frame) const {
	HRESULT hr = S_OK;
	uint32_t pixelBitSize = 0;
	GUID_WICPixelFormat pixelFormat;

	hr = frame->GetPixelFormat(&pixelFormat);
	H::System::ThrowIfFailed(hr);

	auto finded = ImageUtils::StaticData()->WICPixelFormatBitSize.find(pixelFormat);
	if (finded != ImageUtils::StaticData()->WICPixelFormatBitSize.end()) {
		pixelBitSize = finded->second;
	}

	return pixelBitSize;
}

uint32_t ImageUtils::GetPixelBitSize(const GUID_WICPixelFormat &fmt) const {
	uint32_t pixelBitSize = 0;

	auto finded = ImageUtils::StaticData()->WICPixelFormatBitSize.find(fmt);
	if (finded != ImageUtils::StaticData()->WICPixelFormatBitSize.end()) {
		pixelBitSize = finded->second;
	}

	return pixelBitSize;
}

GUID_WICPixelFormat ImageUtils::GetPixelFormat(IWICBitmapSource *frame) const {
	HRESULT hr = S_OK;
	GUID_WICPixelFormat pixelFormat;

	hr = frame->GetPixelFormat(&pixelFormat);
	H::System::ThrowIfFailed(hr);

	return pixelFormat;
}

uint32_t ImageUtils::GetFrameStride(IWICBitmapSource *frame) const {
	uint32_t stride = 0;
	auto frameSize = this->GetFrameSize(frame);
	auto pixelBitSize = this->GetPixelBitSize(frame);

	// formula from https://msdn.microsoft.com/en-us/library/windows/desktop/ee690179%28v=vs.85%29.aspx Codec Developer Remarks
	stride = (frameSize.x * pixelBitSize + 7) / 8;

	return stride;
}

uint32_t ImageUtils::GetFrameByteSize(IWICBitmapSource *frame) const {
	uint32_t byteSize = 0;
	auto frameSize = ImageUtils::GetFrameSize(frame);
	auto pixelBitSize = this->GetPixelBitSize(frame);

	byteSize = (frameSize.x * frameSize.y) * (pixelBitSize / 8);

	return byteSize;
}

DirectX::XMUINT2 ImageUtils::GetFrameSize(IWICBitmapSource *frame) const {
	HRESULT hr = S_OK;
	DirectX::XMUINT2 size;

	hr = frame->GetSize(&size.x, &size.y);
	H::System::ThrowIfFailed(hr);

	return size;
}

ExifRotationFlag ImageUtils::GetExifRotation(IWICBitmapFrameDecode *frame) const {
	HRESULT hr = S_OK;
	ExifRotationFlag exifRotationFlag;
	Microsoft::WRL::ComPtr<IWICMetadataQueryReader> mdReader;
	GUID_ContainerFormat containerFormat;
	PROPVARIANT pv;

	// very important initialize as VT_EMPTY
	// in comments http://stackoverflow.com/questions/14147833/how-do-i-use-the-wic-api-to-read-write-custom-exif-data
	pv.vt = VT_EMPTY;

	hr = frame->GetMetadataQueryReader(mdReader.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = mdReader->GetContainerFormat(&containerFormat);
	H::System::ThrowIfFailed(hr);

	if (containerFormat == GUID_ContainerFormatJpeg) {
		// https://msdn.microsoft.com/en-us/library/windows/desktop/ee719904%28v=vs.85%29.aspx
		hr = mdReader->GetMetadataByName(L"/app1/ifd/{ushort=274}", &pv);
		H::System::ThrowIfFailed(hr);

		exifRotationFlag = static_cast<ExifRotationFlag>(pv.uiVal);
	}
	else {
		exifRotationFlag = ExifRotationFlag::NoTransform;
	}

	return exifRotationFlag;
}

WICBitmapTransformOptions ImageUtils::GetInversedRotationFlipOptions(ExifRotationFlag v) const {
	return ImageUtils::RotationFlipOptionsFromExifInversed(v);
}

WICBitmapTransformOptions ImageUtils::GetRotationFlipOptions(ExifRotationFlag v) const {
	return ImageUtils::RotationFlipOptionsFromExif(v);
}

#if HAVE_WINRT == 1
Microsoft::WRL::ComPtr<IWICBitmapDecoder> ImageUtils::CreateDecoder(Windows::Storage::Streams::IRandomAccessStream ^stream) const {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IStream> istream;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;

	hr = CreateStreamOverRandomAccessStream(stream, IID_PPV_ARGS(istream.GetAddressOf()));
	H::System::ThrowIfFailed(hr);

	hr = this->wicFactory->CreateDecoderFromStream(
		istream.Get(),
		nullptr,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		decoder.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);

	return decoder;
}
#endif

Microsoft::WRL::ComPtr<IWICBitmapDecoder> ImageUtils::CreateDecoder(const std::wstring &path) const {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IStream> istream;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;

	hr = this->wicFactory->CreateDecoderFromFilename(
		path.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		decoder.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);

	return decoder;
}

Microsoft::WRL::ComPtr<IWICBitmapDecoder> ImageUtils::CreateDecoder(const Microsoft::WRL::ComPtr<IStream> &stream) const {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;

	hr = this->wicFactory->CreateDecoderFromStream(
		stream.Get(),
		nullptr,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		decoder.ReleaseAndGetAddressOf());
	H::System::ThrowIfFailed(hr);

	return decoder;
}

Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> ImageUtils::CreateFrameForDecode(IWICBitmapDecoder *decoder, uint32_t idx) const {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;

	hr = decoder->GetFrame(idx, frame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	return frame;
}

void ImageUtils::DecodePixels(IWICBitmapSource *frame, uint32_t pixelsByteSize, void *pixels, const WICRect *rect) const {
	HRESULT hr = S_OK;
	uint32_t frameStride;
	if (rect) {
		frameStride = rect->Width * 4;// this->GetFrameStride(frame);
	}
	else {
		frameStride = this->GetFrameStride(frame);
	}

	hr = frame->CopyPixels(rect, frameStride, pixelsByteSize, static_cast<BYTE *>(pixels));
	H::System::ThrowIfFailed(hr);
}

Microsoft::WRL::ComPtr<IWICBitmap> ImageUtils::LoadToMemory(IWICBitmapSource *frame) const {
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmap> bitmap;

	hr = this->wicFactory->CreateBitmapFromSource(
		frame,
		WICBitmapCreateCacheOption::WICBitmapCacheOnDemand,
		bitmap.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	return bitmap;
}

#if HAVE_WINRT == 1
Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
	Windows::Storage::Streams::IRandomAccessStream ^stream,
	const GUID_ContainerFormat &containerFormat) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IStream> istream;
	Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;

	hr = CreateStreamOverRandomAccessStream(stream, IID_PPV_ARGS(istream.GetAddressOf()));
	H::System::ThrowIfFailed(hr);

	hr = this->wicFactory->CreateEncoder(containerFormat, nullptr, encoder.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = encoder->Initialize(istream.Get(), WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache);
	H::System::ThrowIfFailed(hr);

	return encoder;
}
#else
Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
	const std::wstring &path,
	const GUID_ContainerFormat &containerFormat) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IStream> istream;
	Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;

	hr = SHCreateStreamOnFileW(path.c_str(), STGM_READWRITE, istream.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = this->wicFactory->CreateEncoder(containerFormat, nullptr, encoder.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = encoder->Initialize(istream.Get(), WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache);
	H::System::ThrowIfFailed(hr);

	return encoder;
}
#endif

Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
	const Microsoft::WRL::ComPtr<IStream> &stream,
	const GUID_ContainerFormat &containerFormat) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;

	hr = this->wicFactory->CreateEncoder(containerFormat, nullptr, encoder.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = encoder->Initialize(stream.Get(), WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache);
	H::System::ThrowIfFailed(hr);

	return encoder;
}

Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> ImageUtils::CreateFrameForEncode(IWICBitmapEncoder *encoder) {
	return this->CreateFrameForEncode(encoder, ImageUtilsEncodeNoOptions());
}

void ImageUtils::EncodeAllocPixels(
	IWICBitmapFrameEncode *encodeFrame,
	const DirectX::XMUINT2 &size,
	const GUID_WICPixelFormat &fmt) const
{
	HRESULT hr = S_OK;
	WICPixelFormatGUID pixFmt = fmt;

	hr = encodeFrame->SetSize(size.x, size.y);
	H::System::ThrowIfFailed(hr);

	hr = encodeFrame->SetPixelFormat(&pixFmt);
	H::System::ThrowIfFailed(hr);

	hr = IsEqualGUID(pixFmt, fmt) ? S_OK : E_FAIL;
	H::System::ThrowIfFailed(hr);
}

void ImageUtils::EncodeAllocPixels(
	IWICBitmapFrameEncode *encodeFrame,
	const DirectX::XMUINT2 &size,
	const GUID_WICPixelFormat &fmt,
	GUID_WICPixelFormat &actualFmt) const
{
	HRESULT hr = S_OK;
	actualFmt = fmt;

	hr = encodeFrame->SetSize(size.x, size.y);
	H::System::ThrowIfFailed(hr);

	hr = encodeFrame->SetPixelFormat(&actualFmt);
	H::System::ThrowIfFailed(hr);
}

void ImageUtils::EncodePixels(
	IWICBitmapFrameEncode *encodeFrame,
	uint32_t lineCount, uint32_t stride,
	uint32_t pixelsByteSize, const void *pixels) const
{
	HRESULT hr = S_OK;
	BYTE *pixelsTmp = const_cast<BYTE *>(static_cast<const BYTE *>(pixels));

	hr = encodeFrame->WritePixels(lineCount, stride, pixelsByteSize, pixelsTmp);
	H::System::ThrowIfFailed(hr);
}

void ImageUtils::EncodeFrame(
	IWICBitmapFrameEncode *encodeFrame,
	IWICBitmapSource *frame,
	WICRect *rect) const
{
	HRESULT hr = S_OK;

	hr = encodeFrame->WriteSource(frame, rect);
	H::System::ThrowIfFailed(hr);
}

void ImageUtils::EncodeCommit(IWICBitmapFrameEncode *encodeFrame) const {
	HRESULT hr = S_OK;

	hr = encodeFrame->Commit();
	H::System::ThrowIfFailed(hr);
}

void ImageUtils::EncodeCommit(IWICBitmapEncoder *encoder) const {
	HRESULT hr = S_OK;

	hr = encoder->Commit();
	H::System::ThrowIfFailed(hr);
}

Microsoft::WRL::ComPtr<IWICFormatConverter> ImageUtils::ConvertPixelFormatInMemory(
	IWICBitmapSource *frame,
	const GUID_WICPixelFormat &destFmt
) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICFormatConverter> transformedFrame;
	Microsoft::WRL::ComPtr<IWICBitmap> tmpBitmap = this->LoadToMemory(frame);

	hr = this->wicFactory->CreateFormatConverter(transformedFrame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = transformedFrame->Initialize(tmpBitmap.Get(), destFmt,
		WICBitmapDitherType::WICBitmapDitherTypeNone,
		nullptr, 0.f,
		WICBitmapPaletteType::WICBitmapPaletteTypeCustom);
	H::System::ThrowIfFailed(hr);

	/*Microsoft::WRL::ComPtr<IWICColorTransform> transformedFrame;
	Microsoft::WRL::ComPtr<IWICColorContext> srcColorCtx, dstColorCtx;

	hr = this->wicFactory->CreateColorTransformer(transformedFrame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = this->wicFactory->CreateColorContext(srcColorCtx.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = this->wicFactory->CreateColorContext(dstColorCtx.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = srcColorCtx->InitializeFromExifColorSpace(1);
	H::System::ThrowIfFailed(hr);

	hr = dstColorCtx->InitializeFromExifColorSpace(1);
	H::System::ThrowIfFailed(hr);

	hr = transformedFrame->Initialize(frame, srcColorCtx.Get(), dstColorCtx.Get(), destFmt);
	H::System::ThrowIfFailed(hr);*/

	return transformedFrame;
}

Microsoft::WRL::ComPtr<IWICBitmapScaler> ImageUtils::Scale(
	IWICBitmapSource *frame,
	const DirectX::XMUINT2 &destSize,
	WICBitmapInterpolationMode interpolationMode
) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapScaler> transformedFrame;

	hr = this->wicFactory->CreateBitmapScaler(transformedFrame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = transformedFrame->Initialize(frame, destSize.x, destSize.y, interpolationMode);
	H::System::ThrowIfFailed(hr);

	return transformedFrame;
}

Microsoft::WRL::ComPtr<IWICBitmapFlipRotator> ImageUtils::RotateFlipInMemory(
	IWICBitmapSource *frame,
	WICBitmapTransformOptions options
) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapFlipRotator> transformedFrame;
	/*
	FlipRotator have bug:
	https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/5ff2b52b-602f-4b22-9fb2-371539ff5ebb/hang-in-createbitmapfromwicbitmap-when-using-iwicbitmapfliprotator?forum=windowswic

	Need to load image to IWICBitmap.
	*/
	Microsoft::WRL::ComPtr<IWICBitmap> tmpBitmap = this->LoadToMemory(frame);

	hr = this->wicFactory->CreateBitmapFlipRotator(transformedFrame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = transformedFrame->Initialize(tmpBitmap.Get(), options);
	H::System::ThrowIfFailed(hr);

	return transformedFrame;
}

Microsoft::WRL::ComPtr<IWICBitmapClipper> ImageUtils::Clip(
	IWICBitmapSource *frame,
	const WICRect *prc
) const
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<IWICBitmapClipper> transformedFrame;

	hr = this->wicFactory->CreateBitmapClipper(transformedFrame.GetAddressOf());
	H::System::ThrowIfFailed(hr);

	hr = transformedFrame->Initialize(frame, prc);
	H::System::ThrowIfFailed(hr);

	return transformedFrame;
}

ImagePixels ImageUtils::LoadDxPBgra32Image(const Microsoft::WRL::ComPtr<IStream> &stream, uint32_t maxSize) {
	HRESULT hr = S_OK;
	ImagePixels pixels;
	ImageUtils imgUtils;
	WICBitmapTransformOptions rotateFlip;
	auto dec = imgUtils.CreateDecoder(stream);
	Microsoft::WRL::ComPtr<IWICBitmapSource> frame = imgUtils.CreateFrameForDecode(dec.Get());

	try {
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> decodeFrame;
		frame.As(&decodeFrame);
		auto exifRotation = imgUtils.GetExifRotation(decodeFrame.Get());
		rotateFlip = imgUtils.GetInversedRotationFlipOptions(exifRotation);
	}
	catch (...) {
		rotateFlip = WICBitmapTransformOptions::WICBitmapTransformRotate0;
	}

	pixels.Size = imgUtils.GetFrameSize(frame.Get());

	DirectX::XMFLOAT2 tmpVec((float)pixels.Size.x, (float)pixels.Size.y);
	float scaleFactor = 1.0f;
	if ((float)maxSize < (std::max)(tmpVec.x, tmpVec.y)) {
		scaleFactor = (float)maxSize / (std::max)(tmpVec.x, tmpVec.y);
	}

	tmpVec.x *= scaleFactor;
	tmpVec.y *= scaleFactor;
	pixels.Size = DirectX::XMUINT2((uint32_t)tmpVec.x, (uint32_t)tmpVec.y);
	frame = imgUtils.Scale(frame.Get(), pixels.Size);

	if (rotateFlip != WICBitmapTransformOptions::WICBitmapTransformRotate0) {
		frame = imgUtils.RotateFlipInMemory(frame.Get(), rotateFlip);
	}

	frame = imgUtils.ConvertPixelFormatInMemory(frame.Get(), GUID_WICPixelFormat32bppPBGRA);
	pixels.Size = imgUtils.GetFrameSize(frame.Get());

	auto frameByteSize = imgUtils.GetFrameByteSize(frame.Get());

	pixels.Data.resize(frameByteSize);
	imgUtils.DecodePixels(frame.Get(), frameByteSize, pixels.Data.data());

	return pixels;
}

D2D1_POINT_2U ImageUtils::GetImageSize(const Microsoft::WRL::ComPtr<IStream> &stream) {
	HRESULT hr = S_OK;
	ImageUtils imgUtils;

	auto dec = imgUtils.CreateDecoder(stream);
	Microsoft::WRL::ComPtr<IWICBitmapSource> frame = imgUtils.CreateFrameForDecode(dec.Get());

	D2D1_POINT_2U size;
	frame->GetSize(&size.x, &size.y);

	return size;
}

ImagePixels ImageUtils::LoadDxPBgra32Image(const Microsoft::WRL::ComPtr<IStream> &stream, uint32_t maxSize, WICRect * rect) {//, bool* needsDivision, D2D1_POINT_2U& offset) {
	HRESULT hr = S_OK;
	ImagePixels pixels;
	ImageUtils imgUtils;
	WICBitmapTransformOptions rotateFlip;
	auto dec = imgUtils.CreateDecoder(stream);
	Microsoft::WRL::ComPtr<IWICBitmapSource> frame = imgUtils.CreateFrameForDecode(dec.Get());

	try {
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> decodeFrame;
		frame.As(&decodeFrame);
		auto exifRotation = imgUtils.GetExifRotation(decodeFrame.Get());
		rotateFlip = imgUtils.GetInversedRotationFlipOptions(exifRotation);
	}
	catch (...) {
		rotateFlip = WICBitmapTransformOptions::WICBitmapTransformRotate0;
	}

	/*uint32_t width, height;
	frame->GetSize(&width, &height);
	*/
	//*needsDivision = true;
	//CalculateOffset(offset, width, height, rect->X /*+ rect->Width*/, rect->Y /*+ rect->Height*/, rect->Width, rect->Height);
	//
	//rect->Width -= (width < rect->Width + rect->X)? rect->Width + rect->X - width : 0;
	//rect->Height -= (height < rect->Height + rect->Y)? rect->Height + rect->Y - height: 0;
	//
	//if (offset.y > height) {
	//	*needsDivision = false;
	//}

	pixels.Size = DirectX::XMUINT2(rect->Width, rect->Height);

	if (rotateFlip != WICBitmapTransformOptions::WICBitmapTransformRotate0) {
		frame = imgUtils.RotateFlipInMemory(frame.Get(), rotateFlip);
	}

	frame = imgUtils.ConvertPixelFormatInMemory(frame.Get(), GUID_WICPixelFormat32bppPBGRA);
	//pixels.Size = imgUtils.GetFrameSize(frame.Get());

	auto frameByteSize = imgUtils.GetFrameByteSize(frame.Get());

	pixels.Data.resize(rect->Width * rect->Height * 4);
	imgUtils.DecodePixels(frame.Get(), frameByteSize, pixels.Data.data(), rect);

	return pixels;
}

std::vector<WICRect> ImageUtils::SplitImageRectIntoChunks(D2D1_POINT_2U srcSize, D2D1_POINT_2U chunkSize)
{
	std::vector<WICRect> res;
	WICRect tmp = {0, 0, (INT)chunkSize.x, (INT)chunkSize.y};
	
	do {
		res.push_back(tmp);

		tmp.X += chunkSize.x + 1;
		if (tmp.X > (INT)srcSize.x) {
			tmp.Y += chunkSize.y + 1;
			tmp.X = 0;
		}

		tmp.Width -= ((INT)srcSize.x < tmp.Width + tmp.X) ? tmp.Width + tmp.X - (INT)srcSize.x : 0;
		tmp.Height -= ((INT)srcSize.y < tmp.Height + tmp.Y) ? tmp.Height + tmp.Y - (INT)srcSize.y : 0;
	} while (tmp.Y < (INT)srcSize.y);

	return res;
}

void CalculateOffset(D2D1_POINT_2U& offset, uint32_t srcWidth, uint32_t srcHeight, uint32_t imgRight, uint32_t imgBottom, uint32_t chunkW, uint32_t chunkH) {
	offset.x = imgRight + chunkW + 1;
	if (offset.x > srcWidth) {
		offset.y = imgBottom + chunkH + 1;
		offset.x = 0;
	}
}

WICBitmapTransformOptions ImageUtils::RotationFlipOptionsFromExif(ExifRotationFlag v) {
	// http://www.csharphelper.com/howto_show_exif_orientations.png
	// http://www.impulseadventure.com/photo/exif-orientation.html
	// WICBitmapTransformOptions rotations is clock-wise
	static const WICBitmapTransformOptions Opts[8] = {
		WICBitmapTransformOptions::WICBitmapTransformRotate0,			// 1
		WICBitmapTransformOptions::WICBitmapTransformFlipHorizontal,	// 2
		WICBitmapTransformOptions::WICBitmapTransformRotate180,			// 3
		WICBitmapTransformOptions::WICBitmapTransformFlipVertical,		// 4

		(WICBitmapTransformOptions)
		(WICBitmapTransformOptions::WICBitmapTransformRotate90 |
			WICBitmapTransformOptions::WICBitmapTransformFlipHorizontal),	// 5

		WICBitmapTransformOptions::WICBitmapTransformRotate270,			// 6

		(WICBitmapTransformOptions)
		(WICBitmapTransformOptions::WICBitmapTransformRotate90 |
			WICBitmapTransformOptions::WICBitmapTransformFlipVertical),		// 7

		WICBitmapTransformOptions::WICBitmapTransformRotate90			// 8
	};

	return Opts[static_cast<uint16_t>(v) - 1];
}

WICBitmapTransformOptions ImageUtils::RotationFlipOptionsFromExifInversed(ExifRotationFlag v) {
	// http://www.csharphelper.com/howto_show_exif_orientations.png
	// http://www.impulseadventure.com/photo/exif-orientation.html
	// WICBitmapTransformOptions rotations is clock-wise

	static const WICBitmapTransformOptions Opts[8] = {
		WICBitmapTransformOptions::WICBitmapTransformRotate0,			// 1
		WICBitmapTransformOptions::WICBitmapTransformFlipHorizontal,	// 2
		WICBitmapTransformOptions::WICBitmapTransformRotate180,			// 3
		WICBitmapTransformOptions::WICBitmapTransformFlipVertical,		// 4

		(WICBitmapTransformOptions)
		(WICBitmapTransformOptions::WICBitmapTransformRotate90 |
			WICBitmapTransformOptions::WICBitmapTransformFlipHorizontal),	// 5

		WICBitmapTransformOptions::WICBitmapTransformRotate90,			// 6

		(WICBitmapTransformOptions)
		(WICBitmapTransformOptions::WICBitmapTransformRotate90 |
			WICBitmapTransformOptions::WICBitmapTransformFlipVertical),		// 7

		WICBitmapTransformOptions::WICBitmapTransformRotate270			// 8
	};

	return Opts[static_cast<uint16_t>(v) - 1];
}

void ImageUtils::CreateBitmapFromMemory(UINT width, UINT height, REFWICPixelFormatGUID pixelFormat,
	UINT stride, UINT bufferSize, BYTE* buffer,	IWICBitmap** bmp) {

	HRESULT hr = this->wicFactory->CreateBitmapFromMemory(
		width,
		height,
		pixelFormat,
		stride,
		bufferSize,
		buffer,
		bmp);
	H::System::ThrowIfFailed(hr, L"creating IWICBitmap");
}