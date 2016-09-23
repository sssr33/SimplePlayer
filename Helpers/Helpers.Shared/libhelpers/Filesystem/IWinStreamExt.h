#pragma once

#include <mfobjects.h>
#include <ObjIdl.h>
#include <wrl.h>

namespace Filesystem {
	class IWinStreamExt {
	public:
		IWinStreamExt();
		virtual ~IWinStreamExt();

		virtual Microsoft::WRL::ComPtr<IMFByteStream> CreateIMFByteStream() = 0;
		virtual Microsoft::WRL::ComPtr<::IStream> CreateIStream() = 0;
	};
}