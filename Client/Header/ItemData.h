#pragma once
#include "pch.h"
enum ItemType
{
    FoodMaterial,
    Material,
    Food,
    Weapon,
    None
};
struct Item
{
    int id;
    ItemType type;
    std::string name;
    std::string desc;
    std::wstring UIFileName;
    std::wstring UIPath;


    std::string additionalDesc;

    int quality;
    bool stackable;
    int maxStack;
    int price;

    std::unordered_map<std::string, float> stats;
};
