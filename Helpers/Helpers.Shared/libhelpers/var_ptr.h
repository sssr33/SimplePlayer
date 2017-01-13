#pragma once
#include "Macros.h"

#include <memory>

enum class var_ptr_type {
	unknown,
	unique,
	shared
};

template<class T>
class var_ptr {
public:
	var_ptr_type type;
	std::unique_ptr<T> unique;
	std::shared_ptr<T> shared;

	NO_COPY(var_ptr);

	var_ptr()
		: type(var_ptr_type::unknown), accessPtr(nullptr)
	{
	}

	var_ptr(std::nullptr_t v)
		: type(var_ptr_type::unknown), accessPtr(nullptr)
	{
	}

	template<class T2>
	var_ptr(var_ptr<T2> &&other)
		: type(std::move(other.type)),
		unique(std::move(other.unique)),
		shared(std::move(other.shared))
	{
		this->UpdateAccessPtr();
		other.SetType(var_ptr_type::unknown);
	}

	template<class T2>
	var_ptr(std::unique_ptr<T2> &&other)
		: type(var_ptr_type::unique),
		unique(std::move(other))
	{
		this->UpdateAccessPtr();
	}

	template<class T2>
	var_ptr(std::shared_ptr<T2> &&other)
		: type(var_ptr_type::shared),
		shared(std::move(other))
	{
		this->UpdateAccessPtr();
	}

	template<class T2>
	var_ptr(const std::shared_ptr<T2> &other)
		: type(var_ptr_type::shared),
		shared(other)
	{
		this->UpdateAccessPtr();
	}

	var_ptr &operator=(var_ptr &&other) {
		if (this != &other) {
			this->SetType(std::move(other.type));
			this->unique = std::move(other.unique);
			this->shared = std::move(other.shared);
			this->UpdateAccessPtr();

			other.SetType(var_ptr_type::unknown);
		}

		return *this;
	}

	var_ptr &operator=(std::nullptr_t v) {
		this->SetType(var_ptr_type::unknown);
		this->unique = nullptr;
		this->shared = nullptr;

		return *this;
	}

	template<class T2>
	var_ptr &operator=(std::unique_ptr<T2> &&other) {
		this->SetType(var_ptr_type::unique);
		this->unique = std::move(other);
		this->shared = nullptr;

		return *this;
	}

	template<class T2>
	var_ptr &operator=(std::shared_ptr<T2> &&other) {
		this->SetType(var_ptr_type::shared);
		this->shared = std::move(other);
		this->unique = nullptr;

		return *this;
	}

	template<class T2>
	var_ptr &operator=(const std::shared_ptr<T2> &other) {
		this->SetType(var_ptr_type::shared);
		this->shared = other;
		this->unique = nullptr;

		return *this;
	}

	T *operator->() {
		return this->accessPtr;
	}

	const T *operator->() const {
		return this->accessPtr;
	}

	std::unique_ptr<T> unique_detach() {
		if (this->type == var_ptr_type::unique) {
			this->SetType(var_ptr_type::unknown);
		}

		return std::move(this->unique);
	}

	std::shared_ptr<T> shared_detach() {
		if (this->type == var_ptr_type::shared) {
			this->SetType(var_ptr_type::unknown);
		}

		return std::move(this->shared);
	}

private:
	// used for operator->
	// updated everytime type changes
	T *accessPtr;

	T *GetPtr() const {
		switch (this->type) {
		case var_ptr_type::unique:
			return this->unique.get();
			break;
		case var_ptr_type::shared:
			return this->shared.get();
			break;
		default:
			return nullptr;
		}
	}

	void UpdateAccessPtr() {
		this->accessPtr = this->GetPtr();
	}

	void SetType(var_ptr_type type) {
		this->type = type;
		this->UpdateAccessPtr();
	}

	template<typename> friend class var_ptr;
};

template<class T, class... Types>
var_ptr<T> make_var_ptr(var_ptr_type ptrType, Types&&... args) {

	switch (ptrType) {
	case var_ptr_type::unique:
		return std::make_unique<T>(std::forward<Types>(args) ...);
		break;
	case var_ptr_type::shared:
		return std::make_shared<T>(std::forward<Types>(args) ...);
		break;
	default:
		return nullptr;
	}
}