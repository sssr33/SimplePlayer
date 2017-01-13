#pragma once
#include "IWinStreamSimpleExt.h"

namespace Filesystem {
	class IWinStreamExt : public IWinStreamSimpleExt {
	public:
		IWinStreamExt();
		virtual ~IWinStreamExt();

		IMFByteStream *CreateIMFByteStreamSimple() override;
		::IStream *CreateIStreamSimple() override;

		virtual Microsoft::WRL::ComPtr<IMFByteStream> CreateIMFByteStream() = 0;
		virtual Microsoft::WRL::ComPtr<::IStream> CreateIStream() = 0;
	};
}