#include "pch.h"

#ifdef _DEBUG
#ifdef new
#undef new
#endif
#endif

//  json, fstream 등 순수 C++ 라이브러리 include
#include <fstream>
#include <nlohmann/json.hpp>

//  그 다음에 엔진 쪽 헤더
#include "CItemDB.h"
//#include "Engine_Define.h"   // 필요하다면, 없으면 생략해도 됨

// (원하면) 다시 new 매크로 켜기 ? 이 파일에서도 디버그 new 쓰고 싶으면
#ifdef _DEBUG
#define new DBG_NEW
#endif

IMPLEMENT_SINGLETON(CItemDB)

CItemDB::CItemDB()
{
}

CItemDB::~CItemDB()
{
    Free();
}

HRESULT CItemDB::Ready_ItemDB()
{

    return S_OK;
}

HRESULT CItemDB::LoadFromJson(const string& fileName)
{

    // char buf[MAX_PATH]{};
    // GetCurrentDirectoryA(MAX_PATH, buf);
    // MessageBoxA(nullptr, buf, "Current Directory", MB_OK);
 
    std::ifstream file(fileName, std::ios::in);

    if (!file.is_open())
    {
        //MSG_BOX("itemJson Load Failed");
        return E_FAIL;
    }

    nlohmann::json j;



    try
    {
        file >> j;  // 여기서 문법 이상하면 바로 예외 발생
    }
    catch (const nlohmann::json::parse_error& e)
    {
        MessageBoxA(nullptr, e.what(), "JSON Parse Error", MB_OK);
        return E_FAIL;
    }

    for (auto& node : j["items"])
    {
        Item item; // 구조체 지역으로 생성

        item.id = node["id"];
        item.type = node["type"];
        item.name = node["name"];
        item.desc = node["desc"];
        item.rarity = node["rarity"];
        //JSON에 해당 키가 없을 때 사용할 기본값을 지정 있으면 데이터
        item.stackable = node.value("stackable", false);
        item.maxStack = node.value("maxStack", 1);
        item.price = node.value("price", 0);
        //map stat에 넣을 데이터
        if (node.contains("stats"))
        {
            for (auto& s : node["stats"].items())
                item.stats[s.key()] = s.value();
        }
        m_vecItems.push_back(item);
        m_itemIndex[item.id] = m_vecItems.size() - 1;

    }

    return S_OK;
}

Item* CItemDB::GetItemById(int id)
{
    auto it = m_itemIndex.find(id);
    assert(it != m_itemIndex.end());
    return &m_vecItems[it->second];
}

void CItemDB::Free()
{
}
