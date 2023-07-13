#include "DynamicLibrary.h"

#if WIN32 | WIN64 | WINNT
#include "windows.h"

bool DynamicLibrary::load() {
    libHandle = LoadLibrary(libName);
    return libHandle != nullptr;
}

void DynamicLibrary::free() {
    FreeLibrary((HMODULE)libHandle);
}

void *DynamicLibrary::getFunction(const char *funcName) {
    return (void*)GetProcAddress((HMODULE)libHandle, funcName);
}

#endif