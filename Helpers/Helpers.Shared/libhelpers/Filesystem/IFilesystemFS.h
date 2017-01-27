#pragma once
#include "IFolderFS.h"

namespace Filesystem {
	class IFilesystem {
	public:
		IFilesystem();
		virtual ~IFilesystem();

		var_ptr<IFolder> GetInstalledFolder(var_ptr_type ptrType = var_ptr_type::unique);
		var_ptr<IFolder> GetLocalFolder(var_ptr_type ptrType = var_ptr_type::unique);
		var_ptr<IFile> GetFile(const std::wstring &path, var_ptr_type ptrType = var_ptr_type::unique);
		var_ptr<IFolder> GetFolder(const std::wstring &path, var_ptr_type ptrType = var_ptr_type::unique);

	protected:
		virtual var_ptr<IFolder> GetInstalledFolderI(var_ptr_type ptrType) = 0;
		virtual var_ptr<IFolder> GetLocalFolderI(var_ptr_type ptrType) = 0;
		virtual var_ptr<IFile> GetFileI(const std::wstring &path, var_ptr_type ptrType) = 0;
		virtual var_ptr<IFolder> GetFolderI(const std::wstring &path, var_ptr_type ptrType) = 0;
	};
}