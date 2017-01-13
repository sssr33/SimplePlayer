#include "IFolderFS.h"

namespace Filesystem {
	IFolder::IFolder() {}

	IFolder::~IFolder() {}

	var_ptr<IFile> IFolder::CreateFile(const std::wstring &path, FileCreateMode createMode, var_ptr_type ptrType) {
		return this->CreateFileI(path, createMode, ptrType);
	}

	var_ptr<IFolder> IFolder::CreateFolder(const std::wstring &path, FolderCreateMode createMode, var_ptr_type ptrType) {
		return this->CreateFolderI(path, createMode, ptrType);
	}
}