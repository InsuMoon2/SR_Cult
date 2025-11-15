#include "CScene.h"

#include "CLayer.h"

CScene::CScene(LPDIRECT3DDEVICE9 graphicDev)
    : m_pGraphicDev(graphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{ }

CComponent* CScene::Get_Component(COMPONENTID   componentID,
                                  LAYERTYPE     layerType,
                                  OBJTYPE       objType,
                                  COMPONENTTYPE componentType)
{
    auto iter = m_mapLayer.find(layerType);

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Get_Component(componentID, objType, componentType);
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& timeDelta)
{
    for (auto& pLayer : m_mapLayer)
    {
        pLayer.second->Update_Layer(timeDelta);
    }

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_mapLayer)
    {
        pLayer.second->LateUpdate_Layer(fTimeDelta);
    }
}

void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pGraphicDev);
}
