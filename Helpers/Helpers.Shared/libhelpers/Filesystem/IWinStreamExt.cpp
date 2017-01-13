#include "IWinStreamExt.h"

namespace Filesystem {
	IWinStreamExt::IWinStreamExt() {}

	IWinStreamExt::~IWinStreamExt() {}

	IMFByteStream *IWinStreamExt::CreateIMFByteStreamSimple() {
		auto tmp = this->CreateIMFByteStream();
		return tmp.Detach();
	}

	::IStream *IWinStreamExt::CreateIStreamSimple() {
		auto tmp = this->CreateIStream();
		return tmp.Detach();
	}
}