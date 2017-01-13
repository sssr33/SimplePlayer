#include "IFilesystemFS.h"

namespace Filesystem {
	IFilesystem::IFilesystem() {}

	IFilesystem::~IFilesystem() {}

	var_ptr<IFolder> IFilesystem::GetInstalledFolder(var_ptr_type ptrType) {
		return this->GetInstalledFolderI(ptrType);
	}

	var_ptr<IFolder> IFilesystem::GetLocalFolder(var_ptr_type ptrType) {
		return this->GetLocalFolderI(ptrType);
	}

	var_ptr<IFile> IFilesystem::GetFile(const std::wstring &path, var_ptr_type ptrType) {
		return this->GetFileI(path, ptrType);
	}

	var_ptr<IFolder> IFilesystem::GetFolder(const std::wstring &path, var_ptr_type ptrType) {
		return this->GetFolderI(path, ptrType);
	}
}