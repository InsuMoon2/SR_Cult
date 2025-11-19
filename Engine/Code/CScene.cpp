#include "CScene.h"

#include "CLayer.h"

CScene::CScene(DEVICE graphicDev)
    : m_GraphicDev(graphicDev)
{
    m_GraphicDev->AddRef();
}

CScene::~CScene()
{ }

CComponent* CScene::Get_Component(COMPONENTID   componentID,
                                  LAYERTYPE     layerType,
                                  OBJTYPE       objType,
                                  COMPONENTTYPE componentType)
{
    auto iter = m_Layers.find(layerType);
    
    if (iter == m_Layers.end())
        return nullptr;

    return iter->second->Get_Component(componentID, objType, componentType);
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& timeDelta)
{
    for (auto& pLayer : m_Layers)
    {
        pLayer.second->Update_Layer(timeDelta);
    }

    return 0;
}

void CScene::LateUpdate_Scene(const _float& timeDelta)
{
    for (auto& pLayer : m_Layers)
    {
        pLayer.second->LateUpdate_Layer(timeDelta);
    }
}

void CScene::Free()
{
    for_each(m_Layers.begin(), m_Layers.end(), CDeleteMap());
    m_Layers.clear();

    Safe_Release(m_GraphicDev);
}
