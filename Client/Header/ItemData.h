#pragma once
#include "pch.h"

struct Item
{
    int id;
    std::string type;
    std::string name;
    std::string desc;

    std::string rarity;
    bool stackable;
    int maxStack;
    int price;

    std::unordered_map<std::string, float> stats;
};
