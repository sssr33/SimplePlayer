#pragma once

#define GEN_ENUM_CLASS_BIT_FLAGS_OPERATORS(Class, NumType, EmptyEnumValue)	\
inline Class operator|(const Class &a, const Class &b){						\
	auto tmp = static_cast<NumType>(a) | static_cast<NumType>(b);			\
	return static_cast<Class>(tmp);											\
}																			\
																			\
inline Class operator&(const Class &a, const Class &b){						\
	auto tmp = static_cast<NumType>(a) & static_cast<NumType>(b);			\
	return static_cast<Class>(tmp);											\
}																			\
																			\
inline const Class &operator|=(Class &a, const Class &b){					\
	a = a | b;																\
	return a;																\
}																			\
																			\
inline const Class &operator&=(Class &a, const Class &b){					\
	a = a & b;																\
	return a;																\
}																			\
																			\
inline bool operator!(const Class &a){										\
	return a == EmptyEnumValue;												\
}																			\
																			\
inline Class operator~(const Class &a){										\
	auto tmp = ~static_cast<NumType>(a);									\
	return static_cast<Class>(tmp);											\
}						