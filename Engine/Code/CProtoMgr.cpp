#include "CProtoMgr.h"

IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
    Free();
}

HRESULT CProtoMgr::Ready_Prototype(COMPONENTTYPE eComponentType, CComponent* pComponent)
{
    CComponent* pInstance = Find_Prototype(eComponentType);

    if (nullptr != pInstance)
        return E_FAIL;

    m_mapPrototype.insert({ eComponentType ,pComponent });

    return S_OK;
}

CComponent* CProtoMgr::Clone_Prototype(COMPONENTTYPE eComponentType)
{
    CComponent* pComponent = Find_Prototype(eComponentType);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent->Clone();
}

CComponent* CProtoMgr::Find_Prototype(COMPONENTTYPE eComponentType)
{
    auto iter = m_mapPrototype.find(eComponentType);

    if (iter == m_mapPrototype.end())
        return nullptr;

    return iter->second;
}

void CProtoMgr::Free()
{
    for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
    m_mapPrototype.clear();
}
