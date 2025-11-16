#include "CRenderer.h"

#include "CGameObject.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{ }

CRenderer::~CRenderer()
{
    CRenderer::Free();
}

void CRenderer::Add_RenderGroup(RENDERID type, CGameObject* gameObject)
{
     if (type < RENDER_PRIORITY || type >= RENDER_END || nullptr == gameObject)
        return;

    m_RenderGroup[type].push_back(gameObject);
    gameObject->AddRef();
}

void CRenderer::Render_GameObject(DEVICE& graphicDev)
{
    Render_Priority(graphicDev);
    Render_NonAlpha(graphicDev);
    Render_Alpha(graphicDev);
    Render_UI(graphicDev);

    Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
    for (size_t i = 0; i < RENDER_END; ++i)
    {
        for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
        m_RenderGroup[i].clear();
    }
}

void CRenderer::Render_Priority(DEVICE& graphicDev)
{
    for (auto& obj : m_RenderGroup[RENDER_PRIORITY])
    {
        obj->Render_GameObject();
    }
}

void CRenderer::Render_NonAlpha(DEVICE& graphicDev)
{
    for (auto& obj : m_RenderGroup[RENDER_NONALPHA])
    {
        obj->Render_GameObject();
    }
}

void CRenderer::Render_Alpha(DEVICE& graphicDev)
{
    graphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    graphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    graphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    for (auto& obj : m_RenderGroup[RENDER_ALPHA])
    {
        obj->Render_GameObject();
    }

    graphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(DEVICE& graphicDev)
{
    // TODO 석호: 직교 투영 설정

    for (auto& obj : m_RenderGroup[RENDER_UI])
    {
        obj->Render_GameObject();
    }
}

void CRenderer::Free()
{
    Clear_RenderGroup();
}
