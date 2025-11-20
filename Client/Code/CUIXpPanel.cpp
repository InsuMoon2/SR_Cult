#include "pch.h"
#include "CUIXpPanel.h"
#include "CCreateHelper.h"
#include "CUICircle.h"
#include "CRenderer.h"

CUIXpPanel::CUIXpPanel(DEVICE graphicDev) : CUIPanel(graphicDev)
{
}

CUIXpPanel::CUIXpPanel(const CUIXpPanel& rhs) : CUIPanel(rhs)
{
}

CUIXpPanel::~CUIXpPanel()
{
}

HRESULT CUIXpPanel::Ready_GameObject()
{
    return CUIPanel::Ready_GameObject();

    auto circle = CUICircle::Create(m_GraphicDev);
    NULL_CHECK_RETURN(circle, E_FAIL)
    AddChild(circle);

    return S_OK;
}

_int CUIXpPanel::Update_GameObject(const _float& timeDelta)
{

    _int exit = CUIPanel::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUIXpPanel::LateUpdate_GameObject(const _float& timeDelta)
{
    CUIPanel::LateUpdate_GameObject(timeDelta);
}

void CUIXpPanel::Render_GameObject()
{
    CUIPanel::Render_GameObject();
}


CUIXpPanel* CUIXpPanel::Create(DEVICE graphicDev)
{
    return nullptr;
}

void CUIXpPanel::Free()
{
    CUIPanel::Free();
}
