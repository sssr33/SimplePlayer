#include "IFileFS.h"

namespace Filesystem {
	IFile::IFile() {}

	IFile::~IFile() {}

	var_ptr<IStream> IFile::OpenStream(FileAccessMode accessMode, var_ptr_type ptrType) {
		return this->OpenStreamI(accessMode, ptrType);
	}
}