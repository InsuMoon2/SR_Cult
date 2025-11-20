#pragma once

#include "Engine_Define.h"

BEGIN(Engine)
static const char* ToString(ACTORSTATE state)
{
    switch (state)
    {
    case ACTORSTATE::IDLE:
        return "Idle";

    case ACTORSTATE::RUN:
        return "Run";

    case ACTORSTATE::ATTACK:
        return "Attack";

    case ACTORSTATE::HIT:
        return "Hit";

    case ACTORSTATE::DEAD:
        return "Dead";

    case ACTORSTATE::PLAYERSTATE_END:
    default:
        return "Unknown";
    }
}

static const char* ToString(ACTORDIR dir)
{
    switch (dir)
    {
    case ACTORDIR::LEFT:
        return "Left";

    case ACTORDIR::RIGHT:
        return "Right";

    case ACTORDIR::UP:
        return "Up";

    case ACTORDIR::DOWN:
        return "Down";

    case ACTORDIR::L_UP:
        return "Left Up";

    case ACTORDIR::R_UP:
        return "Right Up";

    case ACTORDIR::L_DOWN:
        return "Left Down";

    case ACTORDIR::R_DOWN:
        return "Right Down";

    default:
        return "Unknown";
    }
}

END
