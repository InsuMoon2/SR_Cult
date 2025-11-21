#include "CLayer.h"

#include "CGameObject.h"
#include "CEnumhelper.h"

CLayer::CLayer()
{ }

CLayer::~CLayer()
{ }

CComponent* CLayer::Get_Component(COMPONENTID componentID, OBJTYPE objType, COMPONENTTYPE componentType)
{
    auto iter = m_Objects.find(objType);

    if (iter == m_Objects.end())
        return nullptr;

    for (auto obj : iter->second)
    {
        CComponent* comp = obj->Get_Component(componentID, componentType);
        if (comp) return comp;
    }

    return nullptr;
}

HRESULT CLayer::Add_GameObject(OBJTYPE objType, CGameObject* gameObject)
{
    if (nullptr == gameObject)
        return E_FAIL;

    const char* baseName = ::ToString(objType);
    wstring wBaseName = CharToWString(baseName);

    // 타입별로 번호 세팅
    // Ex) Monster, Monster2, Monster3..

    _uint index = ++m_NameCounter[objType];

    wstring finalName = L"";

    if (index == 1)
        finalName = wBaseName;

    else
        finalName = wBaseName + L"_" + to_wstring(index);

    gameObject->Set_Name(finalName);

    m_Objects[objType].push_back(gameObject);

    return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
    return S_OK;
}

_int CLayer::Update_Layer(const _float& timeDelta)
{
    _int result = 0;

    for (auto& pair : m_Objects)
    {
        for (auto* obj : pair.second)
        {
            result = obj->Update_GameObject(timeDelta);

            if (result & 0x80000000)
                return result;
        }
    }

    return result;
}

void CLayer::LateUpdate_Layer(const _float& timeDelta)
{
    for (auto& pair : m_Objects)
    {
        for (auto* obj : pair.second)
        {
            obj->LateUpdate_GameObject(timeDelta);
        }
    }
}

CLayer* CLayer::Create()
{
    auto layer = new CLayer;

    if (FAILED(layer->Ready_Layer()))
    {
        MSG_BOX("Layer Create Failed");
        Safe_Release(layer);
        return nullptr;
    }

    return layer;
}

void CLayer::Free()
{
    for (auto& pair : m_Objects)
    {
        for (auto* obj : pair.second)
        {
            Safe_Release(obj);
        }
    }
    m_Objects.clear();
}
