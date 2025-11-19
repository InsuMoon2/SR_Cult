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



END
