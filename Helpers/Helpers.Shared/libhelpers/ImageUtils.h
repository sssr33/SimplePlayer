#pragma once
#include "config.h"
#include "ImageUtilsEncodeOptions.h"
#include "Hashers.h"

#include <wincodec.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <d2d1_2.h>
#include <unordered_map>

#include <Shcore.h>

#if HAVE_WINRT == 0
#include <Shlwapi.h>
#pragma comment( lib, "Shlwapi.lib" )
#endif

typedef GUID GUID_ContainerFormat;
typedef GUID GUID_WICPixelFormat;

// rotations are clock-wise
// exif flags starts from 1
enum class ExifRotationFlag : uint16_t {
	NoTransform = 1,
	FlipHorizontal = 2,
	Rotate180 = 3,
	FlipVertcal = 4,
	Rotate90FlipHorizontal = 5,
	Rotate270 = 6,
	Rotate90FlipVertcal = 7,
	Rotate90 = 8,
};

class ImageUtilsStaticData {
public:
	std::unordered_map<GUID_WICPixelFormat, uint32_t, GUIDHash> WICPixelFormatBitSize;

	ImageUtilsStaticData();
};

struct ImagePixels {
	DirectX::XMUINT2 Size;
	std::vector<uint8_t> Data;
};

class ImageUtils {
	static ImageUtilsStaticData *StaticData();
public:
	ImageUtils();
	~ImageUtils();

	uint32_t GetFrameCount(IWICBitmapDecoder *decoder) const;
	uint32_t GetPixelBitSize(IWICBitmapSource *frame) const;
	uint32_t GetPixelBitSize(const GUID_WICPixelFormat &fmt) const;
	GUID_WICPixelFormat GetPixelFormat(IWICBitmapSource *frame) const;
	uint32_t GetFrameStride(IWICBitmapSource *frame) const;
	uint32_t GetFrameByteSize(IWICBitmapSource *frame) const;
	DirectX::XMUINT2 GetFrameSize(IWICBitmapSource *frame) const;
	ExifRotationFlag GetExifRotation(IWICBitmapFrameDecode *frame) const;
	WICBitmapTransformOptions GetInversedRotationFlipOptions(ExifRotationFlag v) const;
	WICBitmapTransformOptions GetRotationFlipOptions(ExifRotationFlag v) const;

	void CreateBitmapFromMemory(
		UINT width, 
		UINT height,
		REFWICPixelFormatGUID pixelFormat,
		UINT stride,
		UINT bufferSize,
		BYTE* buffer,
		IWICBitmap** bmp);

	// Decoding:
#if HAVE_WINRT == 1
#pragma comment( lib, "shcore.lib" )
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> CreateDecoder(Windows::Storage::Streams::IRandomAccessStream ^stream) const;
#endif
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> CreateDecoder(const std::wstring &path) const;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> CreateDecoder(const Microsoft::WRL::ComPtr<IStream> &stream) const;
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> CreateFrameForDecode(IWICBitmapDecoder *decoder, uint32_t idx = 0) const;

	void DecodePixels(IWICBitmapSource *frame, uint32_t pixelsByteSize, void *pixels, const WICRect *rect = nullptr) const;

	Microsoft::WRL::ComPtr<IWICBitmap> LoadToMemory(IWICBitmapSource *frame) const;

	// Encoding:
#if HAVE_WINRT == 1
	Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
		Windows::Storage::Streams::IRandomAccessStream ^stream,
		const GUID_ContainerFormat &containerFormat) const;
#else
	Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
		const std::wstring &path,
		const GUID_ContainerFormat &containerFormat) const;
#endif

	Microsoft::WRL::ComPtr<IWICBitmapEncoder> ImageUtils::CreateEncoder(
		const Microsoft::WRL::ComPtr<IStream> &stream,
		const GUID_ContainerFormat &containerFormat) const;

	Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> CreateFrameForEncode(IWICBitmapEncoder *encoder);

	template<class OptsImpl>
	Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> CreateFrameForEncode(
		IWICBitmapEncoder *encoder,
		ImageUtilsEncodeOptions<OptsImpl> &options) const
	{
		HRESULT hr = S_OK;
		Microsoft::WRL::ComPtr<IPropertyBag2> props;
		Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> encodeFrame;

		hr = encoder->CreateNewFrame(encodeFrame.GetAddressOf(), props.GetAddressOf());
		H::System::ThrowIfFailed(hr);

		hr = options.Save(props.Get());
		H::System::ThrowIfFailed(hr);

		hr = encodeFrame->Initialize(props.Get());
		H::System::ThrowIfFailed(hr);

		return encodeFrame;
	}

	void EncodeAllocPixels(
		IWICBitmapFrameEncode *encodeFrame,
		const DirectX::XMUINT2 &size,
		const GUID_WICPixelFormat &fmt) const;
	// actualFmt can differ from fmt
	void EncodeAllocPixels(
		IWICBitmapFrameEncode *encodeFrame,
		const DirectX::XMUINT2 &size,
		const GUID_WICPixelFormat &fmt,
		GUID_WICPixelFormat &actualFmt) const;
	void EncodePixels(
		IWICBitmapFrameEncode *encodeFrame,
		uint32_t lineCount, uint32_t stride,
		uint32_t pixelsByteSize, const void *pixels) const;
	void EncodeFrame(
		IWICBitmapFrameEncode *encodeFrame,
		IWICBitmapSource *frame,
		WICRect *rect = nullptr) const;
	void EncodeCommit(IWICBitmapFrameEncode *encodeFrame) const;
	void EncodeCommit(IWICBitmapEncoder *encoder) const;

	// Transforms:

	Microsoft::WRL::ComPtr<IWICFormatConverter> ConvertPixelFormatInMemory(
		IWICBitmapSource *frame,
		const GUID_WICPixelFormat &destFmt
	) const;
	Microsoft::WRL::ComPtr<IWICBitmapScaler> Scale(
		IWICBitmapSource *frame,
		const DirectX::XMUINT2 &destSize,
		WICBitmapInterpolationMode interpolationMode = WICBitmapInterpolationMode::WICBitmapInterpolationModeLinear
	) const;
	Microsoft::WRL::ComPtr<IWICBitmapFlipRotator> RotateFlipInMemory(
		IWICBitmapSource *frame,
		WICBitmapTransformOptions options
	) const;
	Microsoft::WRL::ComPtr<IWICBitmapClipper> Clip(
		IWICBitmapSource *frame,
		const WICRect *prc
	) const;

	// Static helpers:
	// load image and convert to bgra 32 bit with premultiplied alpha
	static ImagePixels LoadDxPBgra32Image(const Microsoft::WRL::ComPtr<IStream> &stream, uint32_t maxSize);
	static D2D1_POINT_2U GetImageSize(const Microsoft::WRL::ComPtr<IStream> &stream);
	static ImagePixels LoadDxPBgra32Image(const Microsoft::WRL::ComPtr<IStream> &stream, uint32_t maxSize, WICRect *rect);// , bool* needsDivision, D2D1_POINT_2U& offset = D2D1::Point2U());

	static std::vector<WICRect> SplitImageRectIntoChunks(D2D1_POINT_2U srcSize, D2D1_POINT_2U chunkSize);

private:
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	wicFactory;

	static WICBitmapTransformOptions RotationFlipOptionsFromExif(ExifRotationFlag v);
	static WICBitmapTransformOptions RotationFlipOptionsFromExifInversed(ExifRotationFlag v);
};