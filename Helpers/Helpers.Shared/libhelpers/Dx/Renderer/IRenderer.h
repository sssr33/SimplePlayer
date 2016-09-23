#pragma once
#include "libhelpers\config.h"
#include "IOutput.h"
#include "../Dx.h"
#include "../../raw_ptr.h"

class IRenderer {
public:
	IRenderer(raw_ptr<DxDevice> dxDev, raw_ptr<IOutput> output);
	virtual ~IRenderer();

	virtual void Render() = 0;
	virtual void OutputParametersChanged() = 0;


protected:
	raw_ptr<DxDevice> dxDev;
	raw_ptr<IOutput> output;
};