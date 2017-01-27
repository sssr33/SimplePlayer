#include "IStreamFS.h"

namespace Filesystem {
	IStream::IStream() {}

	IStream::~IStream() {}

	var_ptr<IStream> IStream::Clone(var_ptr_type ptrType) {
		return this->CloneI(ptrType);
	}
}