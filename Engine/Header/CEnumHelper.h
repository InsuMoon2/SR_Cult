#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

static const char* ToString(PLAYERSTATE state)
{
    switch (state)
    {
    case PLAYERSTATE::IDLE:    return "Idle";
    case PLAYERSTATE::RUN:     return "Run";
    case PLAYERSTATE::ATTACK:  return "Attack";
    case PLAYERSTATE::HIT:     return "Hit";
    case PLAYERSTATE::DEAD:    return "Dead";

    case PLAYERSTATE::PLAYERSTATE_END:
    default:                   return "Unknown";
    }
}

static const char* ToString(PLAYERDIR dir)
{
    switch (dir)
    {
    case PLAYERDIR::UP:        return "Up";
    case PLAYERDIR::DOWN:      return "Down";
    case PLAYERDIR::LEFT:      return "Left";
    case PLAYERDIR::RIGHT:     return "Right";

    case PLAYERDIR::PLAYERDIR_END:
    default:                   return "Unknown";
    }
}

std::wstring Utf8ToWstring(const std::string& str)
{
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0,
        str.c_str(), -1,
        nullptr, 0);

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0,
        str.c_str(), -1,
        &result[0], size_needed);

    // 널문자 제거
    if (!result.empty() && result.back() == L'\0')
        result.pop_back();

    return result;
}

END
