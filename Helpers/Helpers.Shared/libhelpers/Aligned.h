#pragma once

#include <malloc.h>

#pragma warning( push )
// - multiple copy constructors specified
// - disabled because Aligned have templated constructor
// - Aligned(const Aligned &other) is used when copy-constructing 
// object inside const method or from const variable
// - Aligned(Aligned &other) is used in other copy-constructing cases
#pragma warning( disable : 4521 )

// allocates instance of type T in aligned memory
// better use it for simple structures
template<class T, size_t Alignment>
class Aligned {
public:
	template<class... Types>
	Aligned(Types&&... args) {
		this->Alloc();
		new (this->data) T(std::forward<Types>(args)...);
	}

	Aligned(Aligned &other) {
		this->Copy(other);
	}

	Aligned(const Aligned &other) {
		this->Copy(other);
	}

	Aligned(Aligned &&other) {
		this->Move(std::move(other));
	}

	~Aligned() {
		this->Free();
	}

	Aligned &operator=(const Aligned &other) {
		if (this != &other) {
			this->Copy(other);
		}

		return *this;
	}

	Aligned &operator=(Aligned &&other) {
		if (this != &other) {
			this->Free();
			this->Move(std::move(other));
		}

		return *this;
	}

	T *operator->() {
		return this->data;
	}

	const T *operator->() const {
		return this->data;
	}

	T &operator*() {
		return *this->data;
	}

	const T &operator*() const {
		return *this->data;
	}

private:
	T *data;

	void Alloc() {
		this->data = static_cast<T *>(_mm_malloc(sizeof(T), Alignment));
	}

	void Copy(const Aligned &other) {
		if (other.data) {
			// cases:
			// - other.data && this->data
			// - other.data && !this->data
			if (!this->data) {
				this->Alloc();
			}

			// TODO maybe need to call copy constructor of class
			std::memcpy(this->data, other.data, sizeof(T));
		}
		else if (!other.data && this->data) {
			this->Free();
		}
		// else if (!other.data && !this->data) do nothing
	}

	void Move(Aligned &&other) {
		// just copy pointer...
		this->data = other.data;
		// ...and...
		other.data = nullptr;
		// ...to move data
	}

	void Free() {
		if (this->data) {
			this->data->~T();
			_mm_free(this->data);

			// set nullptr in order to detect when re-Alloc is needed
			this->data = nullptr;
		}
	}
};

#pragma warning( pop )