#include "pch.h"
#include "CUIPlayerPanel.h"
#include "CUIHeartBar.h"
#include "CRenderer.h"
#include "CUIPanel.h"

CUIPlayerPanel::CUIPlayerPanel(DEVICE graphicDev):CUIPanel(graphicDev)
{
}

CUIPlayerPanel::CUIPlayerPanel(const CUIPlayerPanel& rhs):CUIPanel(rhs)
{
}

CUIPlayerPanel::~CUIPlayerPanel()
{
}

HRESULT CUIPlayerPanel::Ready_GameObject()
{

    auto image = CUIHeartBar::Create(m_GraphicDev);
    NULL_CHECK_RETURN(image, E_FAIL)
    image->Add_Component();

    AddChild(image); 


    return S_OK;
}

_int CUIPlayerPanel::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUIPanel::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
    return exit;
}
 
void CUIPlayerPanel::LateUpdate_GameObject(const _float& timeDelta)
{
    CUIPanel::LateUpdate_GameObject(timeDelta);
}

void CUIPlayerPanel::Render_GameObject()
{

}

CUIPlayerPanel* CUIPlayerPanel::Create(DEVICE graphicDev)
{
    auto mainmenupanel = new CUIPlayerPanel(graphicDev);

    if (FAILED(mainmenupanel->Ready_GameObject()))
    {
        MSG_BOX("MainMenu Create Failed");
        Safe_Release(mainmenupanel);
        return nullptr;
    }

    return mainmenupanel;
}

void CUIPlayerPanel::Free()
{
    Engine::CUIPanel::Free();
}

