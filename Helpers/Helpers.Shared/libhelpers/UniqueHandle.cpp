#include "UniqueHandle.h"

void UniqueHandleDeleter::operator()(HANDLE handle) {
	CloseHandle(handle);
}

void UniqueFindDeleter::operator()(HANDLE handle) {
	FindClose(handle);
}