#include "CUI.h"

CUI::CUI() : CGameObject(m_GraphicDev)
{
}

CUI::CUI(DEVICE graphicDev) : CGameObject(m_GraphicDev),m_GraphicDev(graphicDev)
{
	m_GraphicDev->AddRef();
}

CUI::CUI(const CUI& rhs) : CGameObject(rhs)
{
	m_GraphicDev->AddRef();
}

CUI::~CUI()
{
}


HRESULT CUI::Ready_GameObject()
{


    return S_OK;
}

_int  CUI::Update_GameObject(const _float& timeDelta)
{

    return S_OK;
}

void  CUI::LateUpdate_GameObject(const _float& timeDelta)
{

}

void  CUI::Render_GameObject()
{

}

void CUI::Free()
{
	Safe_Release(m_GraphicDev);
}
