#include "CUI.h"

CUI::CUI(DEVICE graphicDev)
    : CGameObject(graphicDev)
{

}

CUI::CUI(const CUI& rhs)
    : CGameObject(rhs)
{
 
}

CUI::~CUI()
{}

HRESULT CUI::Ready_GameObject()
{
    CGameObject::Ready_GameObject();

    return S_OK;
}

_int CUI::Update_GameObject(const _float& timeDelta)
{
    CGameObject::Update_GameObject(timeDelta);

    return S_OK;
}

void CUI::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CUI::Render_GameObject()
{
    CGameObject::Render_GameObject();
}

void CUI::Free()
{

}
