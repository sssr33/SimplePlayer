#include "Hashers.h"
#include "HData.h"

size_t GUIDHash::operator()(const GUID &v) const {
	const uint64_t *tmpData4 = reinterpret_cast<const uint64_t *>(v.Data4);

	size_t h1 = std::hash<unsigned long>()(v.Data1);
	size_t h2 = std::hash<unsigned short>()(v.Data2);
	size_t h3 = std::hash<unsigned short>()(v.Data3);
	size_t h4 = std::hash<uint64_t>()(*tmpData4);

	return H::Data::CombineHash(h1, H::Data::CombineHash(h2, H::Data::CombineHash(h3, h4)));
}

size_t XMUINT2Hash::operator()(const DirectX::XMUINT2 &v) const {
	size_t h1 = std::hash<unsigned long>()(v.x);
	size_t h2 = std::hash<unsigned short>()(v.y);

	return H::Data::CombineHash(h1, h2);
}