#pragma once

#include <mfobjects.h>
#include <ObjIdl.h>
#include <wrl.h>

namespace Filesystem {
	class IWinStreamSimpleExt {
	public:
		IWinStreamSimpleExt();
		virtual ~IWinStreamSimpleExt();

		virtual IMFByteStream *CreateIMFByteStreamSimple() = 0;
		virtual ::IStream *CreateIStreamSimple() = 0;
	};
}