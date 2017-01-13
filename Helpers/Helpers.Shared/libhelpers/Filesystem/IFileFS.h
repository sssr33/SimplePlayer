#pragma once
#include "IStreamFS.h"
#include "FileAccessModeFS.h"
#include "FileDeleteModeFS.h"

#include <memory>
#include <string>

namespace Filesystem {
	class IFile {
	public:
		IFile();
		virtual ~IFile();

		virtual uint64_t GetSize() const = 0;
		virtual std::wstring GetPath() const = 0;

		virtual void Delete(FileDeleteMode mode) = 0;

		var_ptr<IStream> OpenStream(FileAccessMode accessMode, var_ptr_type ptrType = var_ptr_type::unique);

	protected:
		virtual var_ptr<IStream> OpenStreamI(FileAccessMode accessMode, var_ptr_type ptrType) = 0;
	};
}