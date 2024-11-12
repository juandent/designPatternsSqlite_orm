#pragma once
#include <chrono>

// Convert time_point to a Unix timestamp (milliseconds since epoch)
inline int64_t toUnixTimestamp(const std::chrono::system_clock::time_point& tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

// Convert a Unix timestamp (milliseconds) back to time_point
inline std::chrono::system_clock::time_point fromUnixTimestamp(int64_t timestamp) {
    return std::chrono::system_clock::time_point{ std::chrono::milliseconds{timestamp} };
}
