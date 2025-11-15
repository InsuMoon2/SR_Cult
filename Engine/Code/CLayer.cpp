#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent* CLayer::Get_Component(COMPONENTID eID, OBJTYPE eObjType, COMPONENTTYPE eComponentType)
{
    auto iter = m_mapObject.find(eObjType);

    if (iter == m_mapObject.end())
        return nullptr;

    return iter->second->Get_Component(eID, eComponentType);
}

HRESULT CLayer::Add_GameObject(OBJTYPE eObjType, CGameObject* pGameObject)
{
    if (nullptr == pGameObject)
        return E_FAIL;

    m_mapObject.insert({ eObjType, pGameObject });

    return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObj : m_mapObject)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_mapObject)
		pObj.second->LateUpdate_GameObject(fTimeDelta);
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
