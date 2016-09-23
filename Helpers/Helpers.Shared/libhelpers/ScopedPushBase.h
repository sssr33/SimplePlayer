#pragma once

// base class for simple destruction based scopes
template<class T>
struct ScopedPushBase {
	T *parent;
};

// wrapper that initializes ScopedPushBaseImpl class
template<class T, class ScopedPushBaseImpl> 
struct ScopedPushWrapper {
	ScopedPushBaseImpl impl;

	ScopedPushWrapper(T *parent) {
		impl.parent = parent;
	}
};