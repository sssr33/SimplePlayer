#include "HData.h"

namespace H {
	size_t Data::CountSetBits(uint8_t v) {
		// source http://stackoverflow.com/a/25808559/2440195
		static const uint8_t NIBBLE_LOOKUP[16] = {
			0, 1, 1, 2, 1, 2, 2, 3,
			1, 2, 2, 3, 2, 3, 3, 4
		};

		return NIBBLE_LOOKUP[v & 0x0F] + NIBBLE_LOOKUP[v >> 4];
	}

	size_t Data::CountSetBits(uint16_t v) {
		const uint8_t *tmp = reinterpret_cast<const uint8_t *>(&v);
		return CountSetBits(tmp[0]) + CountSetBits(tmp[1]);
	}

	size_t Data::CountSetBits(uint32_t v) {
		const uint16_t *tmp = reinterpret_cast<const uint16_t *>(&v);
		return CountSetBits(tmp[0]) + CountSetBits(tmp[1]);
	}

	size_t Data::CountSetBits(uint64_t v) {
		const uint32_t *tmp = reinterpret_cast<const uint32_t *>(&v);
		return CountSetBits(tmp[0]) + CountSetBits(tmp[1]);
	}

	size_t Data::CountSetBits(int8_t v) {
		return Data::CountSetBits(static_cast<uint8_t>(v));
	}

	size_t Data::CountSetBits(int16_t v) {
		return Data::CountSetBits(static_cast<uint16_t>(v));
	}

	size_t Data::CountSetBits(int32_t v) {
		return Data::CountSetBits(static_cast<uint32_t>(v));
	}

	size_t Data::CountSetBits(int64_t v) {
		return Data::CountSetBits(static_cast<uint64_t>(v));
	}

	size_t Data::CountSetBits(const void *data, size_t size) {
		size_t count = 0;
		auto *d_ptr = &data;
		const uint8_t *u8_end = static_cast<const uint8_t *>(data) + size;

		for (const uint64_t **u = reinterpret_cast<const uint64_t **>(d_ptr); (*u + 1) <= reinterpret_cast<const uint64_t *>(u8_end); (*u)++) {
			count += Data::CountSetBits(**u);
		}

		for (const uint32_t **u = reinterpret_cast<const uint32_t **>(d_ptr); (*u + 1) <= reinterpret_cast<const uint32_t *>(u8_end); (*u)++) {
			count += Data::CountSetBits(**u);
		}

		for (const uint16_t **u = reinterpret_cast<const uint16_t **>(d_ptr); (*u + 1) <= reinterpret_cast<const uint16_t *>(u8_end); (*u)++) {
			count += Data::CountSetBits(**u);
		}

		for (const uint8_t **u = reinterpret_cast<const uint8_t **>(d_ptr); *u < u8_end; (*u)++) {
			count += Data::CountSetBits(**u);
		}

		return count;
	}

	void Data::HashRound(uint32_t &hash, uint32_t v) {
		// http://stackoverflow.com/questions/2285822/c-what-is-a-good-way-to-hash-array-data
		hash = (hash + (324723947 + v)) ^ (uint32_t)93485734985;
	}

	size_t Data::CombineHash(size_t a, size_t b) {
		// based on http://en.cppreference.com/w/cpp/utility/hash
		return a ^ (b << 1);
	}
}