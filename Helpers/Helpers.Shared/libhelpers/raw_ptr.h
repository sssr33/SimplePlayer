#pragma once

#include <algorithm>
#include <cstdint>

/*
This class is NOT SMART pointer but just a wrapper around raw pointer.
Main use-cases:
 - PARTIALLY PROTECT pointer FROM DELETION, because for pointers that is need to be deleted more prefferably will be usage of smart pointers.
 - When you need to store or return pointer which MUST be used only as pointer without ability of implicit wrapping in smart pointer.
WARNING : User can delete pointer by using this code:
delete raw_pointer.operator->().operator->();
or
delete raw_pointer.get();
*/
template<class T> class raw_ptr{
public:

	//class accessor{
	//public:
	//	T &operator* () const{
	//		return *p;
	//	}

	//	T *operator->() const{
	//		return this->p;
	//	}

	//	// prefix
	//	accessor& operator++(){
	//		this->p++;
	//		return *this;
	//	}

	//	// prefix
	//	accessor& operator--(){
	//		this->p--;
	//		return *this;
	//	}

	//	accessor operator+(ptrdiff_t v){
	//		accessor tmp(this->p + v);
	//		return tmp;
	//	}

	//	accessor operator-(ptrdiff_t v){
	//		accessor tmp(this->p - v);
	//		return tmp;
	//	}

	//	accessor &operator+=(ptrdiff_t v){
	//		this->p += v;
	//		return *this;
	//	}

	//	accessor &operator-=(ptrdiff_t v){
	//		this->p -= v;
	//		return *this;
	//	}

	//	operator bool() const{
	//		return this->p != nullptr;
	//	}

	//	T *get() const{
	//		return this->p;
	//	}

	//	void reset(T *v){
	//		this->p = v;
	//	}

	//	void reset(T &v){
	//		this->p = &v;
	//	}
	//private:
	//	accessor()
	//		: p(nullptr){
	//	}

	//	accessor(T *p)
	//		: p(p){
	//	}

	//	accessor(const accessor &other)
	//		: p(other.p){
	//	}

	//	accessor(accessor &&other)
	//		: p(other.p){
	//		other.p = nullptr;
	//	}

	//	~accessor(){
	//	}

	//	accessor &operator= (const accessor &other){
	//		if (this != &other){
	//			this->p = other.p;
	//		}

	//		return *this;
	//	}

	//	accessor &operator= (accessor &&other){
	//		if (this != &other){
	//			this->p = other.p;
	//			other.p = nullptr;
	//		}

	//		return *this;
	//	}

	//	T *p;

	//	friend class raw_ptr < T >;
	//};

	raw_ptr()
		: p(nullptr){
	}

	raw_ptr(T &p)
		: p(&p){
	}

	raw_ptr(T *p)
		: p(p){
	}

	raw_ptr(const raw_ptr &other)
		: p(other.p){
	}

	raw_ptr(raw_ptr &&other)
		: p(std::move(other.p)){
	}

	~raw_ptr(){
		// do nothing since this class is used to protect pointer from deletion
	}

	raw_ptr &operator= (const raw_ptr &other){
		if (this != &other){
			this->p = other.p;
		}

		return *this;
	}

	raw_ptr &operator= (raw_ptr &&other){
		if (this != &other){
			this->p = std::move(other.p);
		}

		return *this;
	}

	T &operator* () const{
		return *p;
	}

	/*accessor &operator->() const{
		return this->p;
	}*/

	T *operator->() const{
		return this->p;
	}

	// prefix
	raw_ptr& operator++(){
		++this->p;
		return *this;
	}

	// prefix
	raw_ptr& operator--(){
		--this->p;
		return *this;
	}

	// postfix
	raw_ptr operator++(int){
		raw_ptr tmp(*this);
		++this->p;
		return tmp;
	}

	// postfix
	raw_ptr operator--(int){
		raw_ptr tmp(*this);
		--this->p;
		return tmp;
	}

	raw_ptr operator+(ptrdiff_t v){
		raw_ptr tmp(*this);
		tmp.p += v;
		return tmp;
	}

	raw_ptr operator-(ptrdiff_t v){
		raw_ptr tmp(*this);
		tmp.p -= v;
		return tmp;
	}

	raw_ptr &operator+=(ptrdiff_t v){
		this->p += v;
		return *this;
	}

	raw_ptr &operator-=(ptrdiff_t v){
		this->p -= v;
		return *this;
	}

	template<class T>
	bool operator==(const T &v) const {
		return this->p == v;
	}

	template<class T>
	bool operator!=(const T &v) const {
		return !this->operator==(v);
	}

	operator bool() const{
		return this->p != nullptr;
	}

	T *get() const{
		return this->p;
	}

	void reset(T *v = nullptr){
		this->p.reset(v);
	}

	void reset(T &v){
		this->p.reset(v);
	}
private:
	T *p;
	//accessor p;
};

template<> class raw_ptr<void>{
public:
	raw_ptr()
		: p(nullptr){
	}

	raw_ptr(void *p)
		: p(p){
	}

	raw_ptr(const raw_ptr &other)
		: p(other.p){
	}

	raw_ptr(raw_ptr &&other)
		: p(std::move(other.p)){
	}

	~raw_ptr(){
		// do nothing since this class is used to protect pointer from deletion
	}

	raw_ptr &operator= (const raw_ptr &other){
		if (this != &other){
			this->p = other.p;
		}

		return *this;
	}

	raw_ptr &operator= (raw_ptr &&other){
		if (this != &other){
			this->p = std::move(other.p);
		}

		return *this;
	}

	void *operator->() const{
		return this->p;
	}

	// prefix
	raw_ptr& operator++(){
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = ++bytes;
		return *this;
	}

	// prefix
	raw_ptr& operator--(){
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = --bytes;
		return *this;
	}

	// postfix
	raw_ptr operator++(int){
		raw_ptr tmp(*this);
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = ++bytes;
		return tmp;
	}

	// postfix
	raw_ptr operator--(int){
		raw_ptr tmp(*this);
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = --bytes;
		return tmp;
	}

	raw_ptr operator+(ptrdiff_t v){
		raw_ptr tmp(*this);
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = bytes + v;
		return tmp;
	}

	raw_ptr operator-(ptrdiff_t v){
		raw_ptr tmp(*this);
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = bytes - v;
		return tmp;
	}

	raw_ptr &operator+=(ptrdiff_t v){
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = bytes + v;
		return *this;
	}

	raw_ptr &operator-=(ptrdiff_t v){
		auto bytes = static_cast<uint8_t *>(this->p);
		this->p = bytes - v;
		return *this;
	}

	operator bool() const{
		return this->p != nullptr;
	}

	void *get() const{
		return this->p;
	}
private:
	void *p;
};

template<class T>
raw_ptr<T> make_raw(T &v){
	return raw_ptr<T>(v);
}

template<class T>
raw_ptr<T> make_raw(T *v){
	return raw_ptr<T>(v);
}

template<class T>
const raw_ptr<const T> make_raw(const T &v){
	return raw_ptr<const T>(v);
}

template<class T>
const raw_ptr<const T> make_raw(const T *v){
	return raw_ptr<const T>(v);
}