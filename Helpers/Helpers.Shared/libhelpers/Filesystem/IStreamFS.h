#pragma once
#include "SeekOriginFS.h"
#include "..\PointerWrappers.h"

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

		virtual IStream *Clone() = 0;
	};
}