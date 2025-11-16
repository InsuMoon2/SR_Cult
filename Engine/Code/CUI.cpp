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

void CUI::Free()
{
	Safe_Release(m_pGraphicDev);
}
