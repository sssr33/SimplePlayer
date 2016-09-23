#pragma once
#include "IFolderFS.h"

namespace Filesystem {
	class IFilesystem {
	public:
		IFilesystem();
		virtual ~IFilesystem();

		virtual IFolder *GetInstalledFolder() = 0;
		virtual IFolder *GetLocalFolder() = 0;
		virtual IFile *GetFile(const std::wstring &path) = 0;
		virtual IFolder *GetFolder(const std::wstring &path) = 0;
	};
}