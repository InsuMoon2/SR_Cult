#pragma once
#include "CBase.h"
#include "Engine_Define.h"

#include"ItemData.h"

class CItemDB : public CBase
{
    DECLARE_SINGLETON(CItemDB)
private:
    explicit CItemDB();
    ~CItemDB() override;
public:
    HRESULT Ready_ItemDB();
    HRESULT LoadFromJson(const string& fileName);

    Item*    GetItemById(int id);
    ItemType StringToItemType(string s);
    wstring  ToWString(const std::string& str);

    std::wstring Utf8ToWstring(const std::string& str);

private:
    std::vector<Item>               m_vecItems;
    std::unordered_map<int, size_t> m_itemIndex;
public:
    //	static CItemDB* Create(); 매니져 클래스라 필요없는듯
private:
    void Free() override;
};
