#include "pch.h"
#include "CUIMainMenuPanel.h"
#include "CUIImage.h"
#include "CRenderer.h"
#include "CUIPanel.h"

CUIMainMenuPanel::CUIMainMenuPanel()
{
}

CUIMainMenuPanel::CUIMainMenuPanel(DEVICE graphicDev):CUIPanel(graphicDev)
{
}

CUIMainMenuPanel::CUIMainMenuPanel(const CUIMainMenuPanel& rhs):CUIPanel(rhs)
{
}

CUIMainMenuPanel::~CUIMainMenuPanel()
{
}

HRESULT CUIMainMenuPanel::Ready_GameObject()
{

    auto image = CUIImage::Create(m_GraphicDev);
    NULL_CHECK_RETURN(image, E_FAIL)
    image->Add_Component();

    AddChild(image); // 해당 패널에 포인터 넣기


    return S_OK;
}

_int CUIMainMenuPanel::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUIPanel::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
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
        MSG_BOX("MainMenu Create Failed");
        Safe_Release(mainmenupanel);
        return nullptr;
    }

    return mainmenupanel;
}
