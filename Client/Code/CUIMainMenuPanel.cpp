#include "pch.h"
#include "CUIMainMenuPanel.h"

CUIMainMenuPanel::CUIMainMenuPanel()
{
}

CUIMainMenuPanel::CUIMainMenuPanel(DEVICE graphicDev)
{
}

CUIMainMenuPanel::CUIMainMenuPanel(const CUIMainMenuPanel& rhs)
{
}

CUIMainMenuPanel::~CUIMainMenuPanel()
{
}

HRESULT CUIMainMenuPanel::Ready_GameObject()
{
    return S_OK;
}

_int CUIMainMenuPanel::Update_GameObject(const _float& timeDelta)
{

    _int exit = CUIPanel::Update_GameObject(timeDelta);

    return exit;
}
 
void CUIMainMenuPanel::LateUpdate_GameObject(const _float& timeDelta)
{
    CUIPanel::LateUpdate_GameObject(timeDelta);
}

void CUIMainMenuPanel::Render_GameObject()
{
}

CUIMainMenuPanel* CUIMainMenuPanel::Create(DEVICE graphicDev)
{
    auto mainmenupanel = new CUIMainMenuPanel(graphicDev);

    if (FAILED(mainmenupanel->Ready_GameObject()))
    {
        MSG_BOX("Logo Create Failed");
        Safe_Release(mainmenupanel);
        return nullptr;
    }

    return mainmenupanel;
}
