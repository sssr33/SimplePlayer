#include "DxLayerStack.h"
#include "libhelpers\HSystem.h"

DxLayerStack::PushScope0::~PushScope0() {
	this->parent->PopRenderTarget();
}

DxLayerStack::PushScope1::~PushScope1() {
	this->parent->PopLayer();
}

DxLayerStack::PushScope2::~PushScope2() {
	this->parent->PopAxisAlignedClip();
}

DxLayerStack::DxLayerStack(
	DxDeviceCtxProvider *dxCtxProv,
	DxLayerStackResources *resources)
	: dxCtxProv(dxCtxProv), resources(resources)
{
}

DxLayerStackState DxLayerStack::BeginD3D() {
	HRESULT hr = S_OK;
	uint32_t numViewports = 1;
	D3D11_VIEWPORT viewport;
	D3D11_RECT d3dRect;
	auto d3dCtx = this->dxCtxProv->D3D();
	auto d2dCtx = this->dxCtxProv->D2D();
	auto rect = this->GetCurrentRect();
	auto resourcesState = this->resources->SetToCtx(d3dCtx);
	DxLayerStackState state(std::move(resourcesState), RSScissorState<1>(d3dCtx));

	hr = d2dCtx->Flush();
	H::System::ThrowIfFailed(hr);

	d3dCtx->RSGetViewports(&numViewports, &viewport);

	// fix for
	// D3D11 WARNING: ID3D11DeviceContext::RSSetScissorRects: ScissorRect at slot 0 may not work as intended on some hardware that clamp negative values to 0.
	// must be >= 0
	rect.left = (std::max)(rect.left, 0.0f);
	rect.top = (std::max)(rect.top, 0.0f);
	rect.right = (std::max)(rect.right, 0.0f);
	rect.bottom = (std::max)(rect.bottom, 0.0f);

	// make rect inside viewport
	rect.left = (std::max)(rect.left, viewport.TopLeftX);
	rect.top = (std::max)(rect.top, viewport.TopLeftY);
	rect.right = (std::min)(rect.right, viewport.TopLeftX + viewport.Width);
	rect.bottom = (std::min)(rect.bottom, viewport.TopLeftY + viewport.Height);

	// make leftTop <= rightBottom
	rect.left = (std::min)(rect.left, rect.right);
	rect.top = (std::min)(rect.top, rect.bottom);

	d3dRect.left = (LONG)rect.left;
	d3dRect.top = (LONG)rect.top;
	d3dRect.right = (LONG)rect.right;
	d3dRect.bottom = (LONG)rect.bottom;

	d3dCtx->RSSetScissorRects(1, &d3dRect);

	return state;
}

DxLayerStack::RenderTargetScope DxLayerStack::PushRenderTargetScoped() {
	this->PushRenderTarget();
	return RenderTargetScope(this);
}

void DxLayerStack::PushRenderTarget() {
	HRESULT hr = S_OK;
	auto d2dCtx = this->dxCtxProv->D2D();

	hr = d2dCtx->Flush();
	H::System::ThrowIfFailed(hr);

	this->layerSizesStack.push_back(std::move(this->layerSizes));
}

void DxLayerStack::PopRenderTarget() {
	this->layerSizes = std::move(this->layerSizesStack.back());
	this->layerSizesStack.pop_back();
}

DxLayerStack::LayerScope DxLayerStack::PushLayerScoped(const D2D1_LAYER_PARAMETERS &params, ID2D1Layer *layer) {
	this->PushLayer(params, layer);
	return LayerScope(this);
}

void DxLayerStack::PushLayer(const D2D1_LAYER_PARAMETERS &params, ID2D1Layer *layer) {
	HRESULT hr = S_OK;
	D2D1_MATRIX_3X2_F transform;
	D2D1_RECT_F layerRect = this->GetCurrentRect();
	auto d2dCtx = this->dxCtxProv->D2D();

	d2dCtx->GetTransform(&transform);

	layerRect = DxLayerStack::ConcatRects(
		layerRect, 
		params.contentBounds, 
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, 
		transform);

	if (params.geometricMask) {
		D2D1_RECT_F geomBounds;
		auto geomTransform = transform * params.maskTransform;

		hr = params.geometricMask->GetBounds(geomTransform, &geomBounds);
		H::System::ThrowIfFailed(hr);

		layerRect = DxLayerStack::ConcatRects(layerRect, geomBounds, params.maskAntialiasMode);
	}

	this->layerSizes.push_back(layerRect);
	d2dCtx->PushLayer(params, layer);
}

void DxLayerStack::PopLayer() {
	auto d2dCtx = this->dxCtxProv->D2D();

	this->layerSizes.pop_back();
	d2dCtx->PopLayer();
}

DxLayerStack::AxisAlignedClipScope DxLayerStack::PushAxisAlignedClipScoped(const D2D1_RECT_F &rect, D2D1_ANTIALIAS_MODE antialiasMode) {
	this->PushAxisAlignedClip(rect, antialiasMode);
	return AxisAlignedClipScope(this);
}

void DxLayerStack::PushAxisAlignedClip(const D2D1_RECT_F &rect, D2D1_ANTIALIAS_MODE antialiasMode) {
	HRESULT hr = S_OK;
	D2D1_MATRIX_3X2_F transform;
	D2D1_RECT_F layerRect = this->GetCurrentRect();
	auto d2dCtx = this->dxCtxProv->D2D();

	d2dCtx->GetTransform(&transform);

	layerRect = DxLayerStack::ConcatRects(
		layerRect,
		rect,
		antialiasMode,
		transform);

	this->layerSizes.push_back(layerRect);
	d2dCtx->PushAxisAlignedClip(rect, antialiasMode);
}

void DxLayerStack::PopAxisAlignedClip() {
	auto d2dCtx = this->dxCtxProv->D2D();

	this->layerSizes.pop_back();
	d2dCtx->PopAxisAlignedClip();
}

D2D1_RECT_F DxLayerStack::GetCurrentRect() {
	D2D1_RECT_F rect;

	if (this->layerSizes.empty()) {
		rect = D2D1::InfiniteRect();
	}
	else {
		rect = this->layerSizes.back();
	}

	return rect;
}

D2D1_RECT_F DxLayerStack::ConcatRects(
	D2D1_RECT_F existing,
	D2D1_RECT_F other,
	D2D1_ANTIALIAS_MODE antialiasMode,
	const D2D1_MATRIX_3X2_F &transform)
{
	auto tmpTransform = D2D1::Matrix3x2F::ReinterpretBaseType(&transform);

	if (!DxLayerStack::IsRectInfinite(other)) {
		if (!tmpTransform->IsIdentity()) {
			auto lt = D2D1::Point2F(other.left, other.top);
			auto rt = D2D1::Point2F(other.right, other.top);
			auto lb = D2D1::Point2F(other.left, other.bottom);
			auto rb = D2D1::Point2F(other.right, other.bottom);

			lt = tmpTransform->TransformPoint(lt);
			rt = tmpTransform->TransformPoint(rt);
			lb = tmpTransform->TransformPoint(lb);
			rb = tmpTransform->TransformPoint(rb);

			other.left = (std::min)(rb.x, (std::min)(lb.x, (std::min)(lt.x, rt.x)));
			other.top = (std::min)(rb.y, (std::min)(lb.y, (std::min)(lt.y, rt.y)));
			other.right = (std::max)(rb.x, (std::max)(lb.x, (std::max)(lt.x, rt.x)));
			other.bottom = (std::max)(rb.y, (std::max)(lb.y, (std::max)(lt.y, rt.y)));
		}

		existing = DxLayerStack::ConcatRects(
			existing,
			other,
			antialiasMode);
	}

	return existing;
}

D2D1_RECT_F DxLayerStack::ConcatRects(
	const D2D1_RECT_F &existing,
	const D2D1_RECT_F &other,
	D2D1_ANTIALIAS_MODE antialiasMode)
{
	D2D1_RECT_F result;

	result.left = (std::max)(existing.left, other.left);
	result.left = (std::min)(result.left, existing.right);

	result.top = (std::max)(existing.top, other.top);
	result.top = (std::min)(result.top, existing.bottom);

	result.right = (std::min)(existing.right, other.right);
	result.right = (std::max)(result.right, existing.left);

	result.bottom = (std::min)(existing.bottom, other.bottom);
	result.bottom = (std::max)(result.bottom, existing.top);

	// TODO test if there will be differences 
	// for D2D1_ANTIALIAS_MODE_PER_PRIMITIVE and D2D1_ANTIALIAS_MODE_ALIASED
	// but few tests have shown that current logic works
	/*switch (antialiasMode) {
	case D2D1_ANTIALIAS_MODE_PER_PRIMITIVE:
		result.left = std::floorf(result.left);
		result.top = std::floorf(result.top);
		result.right = std::ceilf(result.right);
		result.bottom = std::ceilf(result.bottom);
		break;
	case D2D1_ANTIALIAS_MODE_ALIASED:
		break;
	default:
		break;
	}*/

	result.left = std::floorf(result.left);
	result.top = std::floorf(result.top);
	result.right = std::ceilf(result.right);
	result.bottom = std::ceilf(result.bottom);

	return result;
}

bool DxLayerStack::IsRectInfinite(const D2D1_RECT_F &v) {
	const auto infRect = D2D1::InfiniteRect();
	bool res =
		v.left == infRect.left &&
		v.top == infRect.top &&
		v.right == infRect.right &&
		v.bottom == infRect.bottom;

	return res;
}