#pragma once

#include <memory>

template<class T>
std::unique_ptr<T> Unique(T *ptr) {
	return std::unique_ptr<T>(ptr);
}

template<class T>
std::shared_ptr<T> Shared(T *ptr) {
	return std::shared_ptr<T>(ptr);
}