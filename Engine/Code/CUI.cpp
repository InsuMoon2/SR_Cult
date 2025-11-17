#include "CUI.h"

CUI::CUI() : CGameObject(m_pGraphicDev)
{
}

CUI::CUI(DEVICE pGraphicDev) : CGameObject(m_pGraphicDev),m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CUI::CUI(const CUI& rhs) : CGameObject(rhs)
{
	m_pGraphicDev->AddRef();
}

CUI::~CUI()
{
}


HRESULT Ready_GameObject()
{

}

_int  CUI::Update_GameObject(const _float& timeDelta)
{

}

void  CUI::LateUpdate_GameObject(const _float& timeDelta)
{

}

void  CUI::Render_GameObject()
{

}

void CUI::Free()
{
	Safe_Release(m_pGraphicDev);
}
