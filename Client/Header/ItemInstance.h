#pragma once

struct ItemInstance
{
    int itemId = -1; // 아이템 id (필수)

    // 플레이어
    int count = 0;
    int durability = -1;
    float extraAttack = 0;
};
