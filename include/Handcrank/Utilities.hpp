// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <algorithm>
#include <cxxabi.h>
#include <string>

namespace Handcrank
{

inline auto LeftPad(const std::string &content, const char pad, int length)
    -> std::string
{
    return std::string(length - content.size(), pad) + content;
}

inline auto RightPad(const std::string &content, const char pad, int length)
    -> std::string
{
    return content + std::string(length - content.size(), pad);
}

inline auto Lerp(float a, float b, float t) -> float
{
    return ((1 - t) * a) + (b * t);
}

inline auto InverseLerp(float a, float b, float v) -> float
{
    return std::clamp(((v - a) / (b - a)), 0.0F, 1.0F);
}

inline auto RandomNumberRange(int min, int max) -> int
{
    return (rand() % (max - min + 1)) + min;
}

inline auto RandomBoolean() -> bool { return rand() > (RAND_MAX / 2); }

template <typename T> auto GetDemangledClassName(const T &obj) -> std::string
{
    const auto &info = typeid(obj);

    int status;

    auto *demangled =
        abi::__cxa_demangle(info.name(), nullptr, nullptr, &status);

    std::string result;

    if (status == 0 && demangled)
    {
        result = demangled;

        free(demangled);
    }
    else
    {
        result = info.name();
    }

    return result;
}

} // namespace Handcrank
