#include "IFilesystemFS.h"
#include "IFolderFS.h"
#include "IFileFS.h"
#include "IStreamFS.h"
#include "IWinStreamExt.h"

#undef CreateFile

namespace Filesystem {
	IFilesystem::IFilesystem() {
	}

	IFilesystem::~IFilesystem() {
	}

	IFolder::IFolder() {
	}

	IFolder::~IFolder() {
	}

	IFile::IFile() {
	}

	IFile::~IFile() {
	}

	IStream::IStream() {
	}

	IStream::~IStream() {
	}

	IWinStreamExt::IWinStreamExt() {
	}

	IWinStreamExt::~IWinStreamExt() {
	}
}