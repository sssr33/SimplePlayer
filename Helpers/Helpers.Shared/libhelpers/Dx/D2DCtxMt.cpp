#include "D2DCtxMt.h"

D2DCtxMt::D2DCtxMt() {
}

D2DCtxMt::D2DCtxMt(const Microsoft::WRL::ComPtr<ID2D1DeviceContext> &d2dCtx)
	: d2dCtx(d2dCtx) {
}

// CreateBitmap
HRESULT D2DCtxMt::CreateBitmap(
	D2D1_SIZE_U size,
	_In_opt_ CONST void *srcData,
	UINT32 pitch,
	CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmap(
	D2D1_SIZE_U size,
	_In_opt_ CONST void *srcData,
	UINT32 pitch,
	_In_ CONST D2D1_BITMAP_PROPERTIES *bitmapProperties,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmap(size, srcData, pitch, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmap(
	D2D1_SIZE_U size,
	CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmap(size, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmap(
	D2D1_SIZE_U size,
	_In_opt_ CONST void *sourceData,
	UINT32 pitch,
	CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmap(size, sourceData, pitch, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmap(
	D2D1_SIZE_U size,
	_In_opt_ CONST void *sourceData,
	UINT32 pitch,
	_In_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmap(size, sourceData, pitch, bitmapProperties, bitmap);
}

// CreateBitmapBrush
HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	_Outptr_ ID2D1BitmapBrush **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	CONST D2D1_BITMAP_BRUSH_PROPERTIES &bitmapBrushProperties,
	_Outptr_ ID2D1BitmapBrush **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	CONST D2D1_BITMAP_BRUSH_PROPERTIES &bitmapBrushProperties,
	CONST D2D1_BRUSH_PROPERTIES &brushProperties,
	_Outptr_ ID2D1BitmapBrush **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	_In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES *bitmapBrushProperties,
	_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
	_Outptr_ ID2D1BitmapBrush **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	_Outptr_ ID2D1BitmapBrush1 **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	CONST D2D1_BITMAP_BRUSH_PROPERTIES1 &bitmapBrushProperties,
	_Outptr_ ID2D1BitmapBrush1 **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	CONST D2D1_BITMAP_BRUSH_PROPERTIES1 &bitmapBrushProperties,
	CONST D2D1_BRUSH_PROPERTIES &brushProperties,
	_Outptr_ ID2D1BitmapBrush1 **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
}

HRESULT D2DCtxMt::CreateBitmapBrush(
	_In_opt_ ID2D1Bitmap *bitmap,
	_In_opt_ CONST D2D1_BITMAP_BRUSH_PROPERTIES1 *bitmapBrushProperties,
	_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
	_Outptr_ ID2D1BitmapBrush1 **bitmapBrush)
{
	return this->d2dCtx->CreateBitmapBrush(bitmap, bitmapBrushProperties, brushProperties, bitmapBrush);
}

// CreateBitmapFromDxgiSurface
HRESULT D2DCtxMt::CreateBitmapFromDxgiSurface(
	_In_ IDXGISurface *surface,
	CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmapFromDxgiSurface(surface, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromDxgiSurface(
	_In_ IDXGISurface *surface,
	_In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap) 
{
	return this->d2dCtx->CreateBitmapFromDxgiSurface(surface, bitmapProperties, bitmap);
}

// CreateBitmapFromWicBitmap
HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	CONST D2D1_BITMAP_PROPERTIES &bitmapProperties,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	_In_opt_ CONST D2D1_BITMAP_PROPERTIES *bitmapProperties,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	_Outptr_ ID2D1Bitmap **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	CONST D2D1_BITMAP_PROPERTIES1 &bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	_In_opt_ CONST D2D1_BITMAP_PROPERTIES1 *bitmapProperties,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmapProperties, bitmap);
}

HRESULT D2DCtxMt::CreateBitmapFromWicBitmap(
	_In_ IWICBitmapSource *wicBitmapSource,
	_Outptr_ ID2D1Bitmap1 **bitmap)
{
	return this->d2dCtx->CreateBitmapFromWicBitmap(wicBitmapSource, bitmap);
}

// CreateSolidColorBrush
HRESULT D2DCtxMt::CreateSolidColorBrush(
	CONST D2D1_COLOR_F &color,
	_Outptr_ ID2D1SolidColorBrush **solidColorBrush)
{
	return this->d2dCtx->CreateSolidColorBrush(color, solidColorBrush);
}

HRESULT D2DCtxMt::CreateSolidColorBrush(
	CONST D2D1_COLOR_F &color,
	CONST D2D1_BRUSH_PROPERTIES &brushProperties,
	_Outptr_ ID2D1SolidColorBrush **solidColorBrush)
{
	return this->d2dCtx->CreateSolidColorBrush(color, brushProperties, solidColorBrush);
}

HRESULT D2DCtxMt::CreateSolidColorBrush(
	_In_ CONST D2D1_COLOR_F *color,
	_In_opt_ CONST D2D1_BRUSH_PROPERTIES *brushProperties,
	_Outptr_ ID2D1SolidColorBrush **solidColorBrush)
{
	return this->d2dCtx->CreateSolidColorBrush(color, brushProperties, solidColorBrush);
}

// CreateEffect
HRESULT D2DCtxMt::CreateEffect(
	_In_ REFCLSID effectId,
	_Outptr_ ID2D1Effect **effect
)
{
	return this->d2dCtx->CreateEffect(effectId, effect);
}

void D2DCtxMt::GetDpi(
	_Out_ FLOAT* dpiX,
	_Out_ FLOAT* dpiY
) {
	this->d2dCtx->GetDpi(dpiX, dpiY);
}

// CreateLayer
HRESULT D2DCtxMt::CreateLayer(
	D2D1_SIZE_F size,
	_Outptr_ ID2D1Layer **layer)
{
	return this->d2dCtx->CreateLayer(size, layer);
}

HRESULT D2DCtxMt::CreateLayer(
	_Outptr_ ID2D1Layer **layer)
{
	return this->d2dCtx->CreateLayer(layer);
}