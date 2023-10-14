#pragma once

#include "logger.h"
#include <coreinit/debug.h>
#include <memory>
#include <string>

template<typename T>
inline typename std::unique_ptr<T> make_unique_nothrow(size_t num) noexcept {
    return std::unique_ptr<T>(new (std::nothrow) std::remove_extent_t<T>[num]());
}

template<typename... Args>
std::string string_format(const std::string &format, Args... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    auto size  = static_cast<size_t>(size_s);
    auto buf   = make_unique_nothrow<char[]>(size);
    if (!buf) {
        DEBUG_FUNCTION_LINE_ERR("string_format failed, not enough memory");
        OSFatal("string_format failed, not enough memory");
        return std::string("");
    }
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
