#pragma once
#include "pch.h"

struct Item
{
    int id;
    std::string type;
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
