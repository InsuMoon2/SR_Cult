#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{ }

CLayer::~CLayer()
{ }

CComponent* CLayer::Get_Component(COMPONENTID componentID, OBJTYPE objType, COMPONENTTYPE componentType)
{
    auto iter = m_Objects.find(objType);

    if (iter == m_Objects.end())
        return nullptr;

    return iter->second->Get_Component(componentID, componentType);
}

HRESULT CLayer::Add_GameObject(OBJTYPE objType, CGameObject* gameObject)
{
    if (nullptr == gameObject)
        return E_FAIL;

    m_Objects.insert({ objType, gameObject });

    return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
    return S_OK;
}

_int CLayer::Update_Layer(const _float& timeDelta)
{
    _int result = 0;

    for (auto& obj : m_Objects)
    {
        result = obj.second->Update_GameObject(timeDelta);

        if (result & 0x80000000)
            return result;
    }

    return result;
}

void CLayer::LateUpdate_Layer(const _float& timeDelta)
{
    for (auto& obj : m_Objects)
    {
        obj.second->LateUpdate_GameObject(timeDelta);
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
    for_each(m_Objects.begin(), m_Objects.end(), CDeleteMap());
    m_Objects.clear();
}
