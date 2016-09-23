#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wrl.h>

// part of direct2d device context that can be used from multiple threads
class D2DCtxMt {
public:
	D2DCtxMt();
	D2DCtxMt(const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx);

	// CreateBitmap
	HRESULT CreateBitmap(
			D2D1_SIZE_U size,
			_In_opt_ CONST void *srcData,
			UINT32 pitch,
			CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
			_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmap(
			D2D1_SIZE_U size,
			_In_opt_ CONST void *srcData,
			UINT32 pitch,
			_In_ CONST D2D1_BITMAP_PROPERTIES *bitmapProperties,
			_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmap(
			D2D1_SIZE_U size,
			CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
			_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmap(
			D2D1_SIZE_U size,
			_In_opt_ CONST void *sourceData,
			UINT32 pitch,
			CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	HRESULT CreateBitmap(
			D2D1_SIZE_U size,
			_In_opt_ CONST void *sourceData,
			UINT32 pitch,
			_In_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	// CreateBitmapBrush
	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			_Outptr_ ID2D1BitmapBrush **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			CONST D2D1_BITMAP_BRUSH_PROPERTIES &bitmapBrushProperties,
			_Outptr_ ID2D1BitmapBrush **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			CONST D2D1_BITMAP_BRUSH_PROPERTIES &bitmapBrushProperties,
			CONST D2D1_BRUSH_PROPERTIES &brushProperties,
			_Outptr_ ID2D1BitmapBrush **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			_In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES *bitmapBrushProperties,
			_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
			_Outptr_ ID2D1BitmapBrush **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			_Outptr_ ID2D1BitmapBrush1 **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			CONST D2D1_BITMAP_BRUSH_PROPERTIES1 &bitmapBrushProperties,
			_Outptr_ ID2D1BitmapBrush1 **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			CONST D2D1_BITMAP_BRUSH_PROPERTIES1 &bitmapBrushProperties,
			CONST D2D1_BRUSH_PROPERTIES &brushProperties,
			_Outptr_ ID2D1BitmapBrush1 **bitmapBrush);

	HRESULT CreateBitmapBrush(
			_In_opt_ ID2D1Bitmap *bitmap,
			_In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 *bitmapBrushProperties,
			_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
			_Outptr_ ID2D1BitmapBrush1 **bitmapBrush);

	// CreateBitmapFromDxgiSurface
	HRESULT CreateBitmapFromDxgiSurface(
			_In_ IDXGISurface *surface,
			CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	HRESULT CreateBitmapFromDxgiSurface(
			_In_ IDXGISurface *surface,
			_In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	// CreateBitmapFromWicBitmap
	HRESULT CreateBitmapFromWicBitmap(
			_In_ IWICBitmapSource *wicBitmapSource,
			CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
			_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmapFromWicBitmap(
		_In_ IWICBitmapSource *wicBitmapSource,
		_In_opt_ CONST D2D1_BITMAP_PROPERTIES *bitmapProperties,
		_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmapFromWicBitmap(
			_In_ IWICBitmapSource *wicBitmapSource,
			_Outptr_ ID2D1Bitmap **bitmap);

	HRESULT CreateBitmapFromWicBitmap(
			_In_ IWICBitmapSource *wicBitmapSource,
			CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	HRESULT CreateBitmapFromWicBitmap(
		_In_ IWICBitmapSource *wicBitmapSource,
		_In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
		_Outptr_ ID2D1Bitmap1 **bitmap);

	HRESULT CreateBitmapFromWicBitmap(
			_In_ IWICBitmapSource *wicBitmapSource,
			_Outptr_ ID2D1Bitmap1 **bitmap);

	// CreateSolidColorBrush
	HRESULT CreateSolidColorBrush(
			CONST D2D1_COLOR_F &color,
			_Outptr_ ID2D1SolidColorBrush **solidColorBrush);

	HRESULT CreateSolidColorBrush(
			CONST D2D1_COLOR_F &color,
			CONST D2D1_BRUSH_PROPERTIES &brushProperties,
			_Outptr_ ID2D1SolidColorBrush **solidColorBrush);

	HRESULT CreateSolidColorBrush(
		_In_ CONST D2D1_COLOR_F *color,
		_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
		_Outptr_ ID2D1SolidColorBrush **solidColorBrush);

	// CreateEffect
	HRESULT CreateEffect(
		_In_ REFCLSID effectId,
		_Outptr_ ID2D1Effect **effect
		);

private:
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dCtx;
};