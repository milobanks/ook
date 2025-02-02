#include <sys/libkern/String.hpp>

using namespace ook;

size_t libkern::string_length(const char* cstr) {
    const char* ptr;
    for (ptr = cstr; *ptr; ptr++) {}
    return ptr - cstr;
}
