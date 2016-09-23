#pragma once

#define WSTR1(x) L#x
#define WSTR2(x) WSTR1(x)
#define WSCONCAT(a, b) WSTR2(a) WSTR2(b)
#define ARRAY_SIZE(A) sizeof A / sizeof A[0]

#define NO_COPY(className) \
	className(const className &) = delete; \
	className &operator=(const className &) = delete; \

#define NO_COPY_MOVE(className) \
    NO_COPY(className) \
    className(className &&) = delete; \
	className &operator=(className &&) = delete;