#pragma once
#include "SeekOriginFS.h"
#include "..\var_ptr.h"

#include <cstdint>
#include <memory>

namespace Filesystem {
	class IStream {
	public:
		IStream();
		virtual ~IStream();

		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;

		virtual uint64_t GetSize() const = 0;
		virtual uint64_t GetPosition() const = 0;

		virtual uint32_t Read(void *buf, uint32_t length) = 0;
		virtual uint32_t Write(const void *buf, uint32_t length) = 0;
		virtual void Seek(SeekOrigin origin, int64_t offset) = 0;

		var_ptr<IStream> Clone(var_ptr_type ptrType = var_ptr_type::unique);

	protected:
		virtual var_ptr<IStream> CloneI(var_ptr_type ptrType) = 0;
	};
}