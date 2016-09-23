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

		virtual IFile *CreateFile(const std::wstring &path, FileCreateMode createMode) = 0;
		virtual IFolder *CreateFolder(const std::wstring &path, FolderCreateMode createMode) = 0;
	};
}