#pragma once
#include "IFileFS.h"
#include "CreateModeFS.h"

#include <string>

#undef CreateFile

namespace Filesystem {
	class IFolder {
	public:
		IFolder();
		virtual ~IFolder();

		virtual std::wstring GetPath() const = 0;

		var_ptr<IFile> CreateFile(const std::wstring &path, FileCreateMode createMode, var_ptr_type ptrType = var_ptr_type::unique);
		var_ptr<IFolder> CreateFolder(const std::wstring &path, FolderCreateMode createMode, var_ptr_type ptrType = var_ptr_type::unique);

	protected:
		virtual var_ptr<IFile> CreateFileI(const std::wstring &path, FileCreateMode createMode, var_ptr_type ptrType) = 0;
		virtual var_ptr<IFolder> CreateFolderI(const std::wstring &path, FolderCreateMode createMode, var_ptr_type ptrType) = 0;
	};
}