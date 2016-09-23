#pragma once

#include <wrl.h>
#include <vector>
#include <type_traits>
#include <algorithm>

template<class T>
class comptr_vector {
	static_assert(std::is_base_of<IUnknown, T>::value, "T must inherit from IUnknown");
	typedef std::vector<T *> containerType;
public:
	typedef typename containerType::pointer pointer;
	typedef typename containerType::const_pointer const_pointer;
	typedef typename containerType::const_reverse_iterator const_reverse_iterator;
	typedef typename containerType::const_iterator const_iterator;
	typedef typename containerType::const_reverse_iterator const_reverse_iterator;
	typedef typename containerType::iterator iterator;
	typedef typename containerType::reverse_iterator reverse_iterator;

	comptr_vector() {
	}

	comptr_vector(size_t size)
		: container(size, nullptr) {
	}

	comptr_vector(const comptr_vector &other)
		: container(other.container)
	{
		this->AddDataRef();
	}

	comptr_vector(comptr_vector &&other)
		: container(std::move(other.container)) {
	}

	~comptr_vector() {
		this->ReleaseDataRef();
	}

	comptr_vector &operator=(const comptr_vector &other) {
		if (this != &other) {
			this->container = other.container;

			this->AddDataRef();
		}

		return *this;
	}

	comptr_vector &operator=(comptr_vector &&other) {
		if (this != &other) {
			this->container = std::move(other.container);
		}

		return *this;
	}

	T * const&operator[](size_t idx) const {
		return this->container[idx];
	}

	T *&operator[](size_t idx) {
		return this->container[idx];
	}

	size_t size() const {
		return this->container.size();
	}

	pointer data() {
		return this->container.data();
	}

	const_pointer data() const {
		return this->container.data();
	}

	iterator begin() {
		return this->container.begin();
	}

	const_iterator begin() const {
		return this->container.begin();
	}

	iterator end() {
		return this->container.end();
	}

	const_iterator end() const {
		return this->container.end();
	}

	void push_back(const Microsoft::WRL::ComPtr<T> &v) {
		auto tmp = v.Get();
		if (tmp) {
			static_cast<IUnknown *>(tmp)->AddRef();
		}

		this->container.push_back(tmp);
	}

	void push_back(T *v) {
		if (v) {
			static_cast<IUnknown *>(v)->AddRef();
		}

		this->container.push_back(v);
	}

	void reserve(size_t v) {
		this->container.reserve(v);
	}

	void resize(size_t v) {
		this->container.resize(v, nullptr);
	}
private:
	containerType container;

	void AddDataRef() {
		for (auto &i : this->container) {
			if (i) {
				static_cast<IUnknown *>(i)->AddRef();
			}
		}
	}

	void ReleaseDataRef() {
		for (auto &i : this->container) {
			if (i) {
				static_cast<IUnknown *>(i)->Release();
				i = nullptr;
			}
		}
	}
};