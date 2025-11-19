#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

static const char* ToString(ACTORSTATE state)
{
    switch (state)
    {
    case ACTORSTATE::IDLE:    return "Idle";
    case ACTORSTATE::RUN:     return "Run";
    case ACTORSTATE::ATTACK:  return "Attack";
    case ACTORSTATE::HIT:     return "Hit";
    case ACTORSTATE::DEAD:    return "Dead";

    case ACTORSTATE::PLAYERSTATE_END:
    default:                   return "Unknown";
    }
}

static const char* ToString(ACTORDIR dir)
{
    switch (dir)
    {
    case ACTORDIR::UP:        return "Up";
    case ACTORDIR::DOWN:      return "Down";
    case ACTORDIR::LEFT:      return "Left";
    case ACTORDIR::RIGHT:     return "Right";

    case ACTORDIR::PLAYERDIR_END:
    default:                   return "Unknown";
    }
}



END
