#pragma once

#include <algorithm>
#include <exception>

template<class T> class result{
public:

	result(){
	}

	result(result &&other)
		: res(std::move(other.res)),
		exception(std::move(other.exception)){
	}

	result &operator= (result &&other){
		if (this != &other){
			this->res = std::move(other.res);
			this->exception = std::move(other.exception);
		}

		return *this;
	}

	const T &get() const{
		if (this->exception){
			std::rethrow_exception(this->exception);
		}
		return this->res;
	}

	T get_move(){
		if (this->exception){
			std::rethrow_exception(this->exception);
		}
		return std::move(this->res);
	}

	void set_result(const T &v){
		this->res = v;
	}

	void set_result(T &&v){
		this->res = std::move(v);
	}

	void set_exception(const std::exception_ptr &e){
		this->exception = e;
	}

	void set_exception(const std::exception &e){
		this->exception = std::make_exception_ptr(e);
	}
private:
	T res;
	std::exception_ptr exception;
};

template<> class result<void>{
public:

	result(){
	}

	result(result &&other)
		: exception(std::move(other.exception)){
	}

	result &operator= (result &&other){
		if (this != &other){
			this->exception = std::move(other.exception);
		}

		return *this;
	}

	void get() const{
		if (this->exception){
			std::rethrow_exception(this->exception);
		}
	}

	void get_move(){
		if (this->exception){
			std::rethrow_exception(this->exception);
		}
	}

	void set_result(){
	}

	void set_exception(const std::exception_ptr &e){
		this->exception = e;
	}

	void set_exception(const std::exception &e){
		this->exception = std::make_exception_ptr(e);
	}
private:
	std::exception_ptr exception;
};