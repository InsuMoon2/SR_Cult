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

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& graphicDev)
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

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& graphicDev)
{
    for (auto& obj : m_RenderGroup[RENDER_PRIORITY])
    {
        obj->Render_GameObject();
    }
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& graphicDev)
{
    for (auto& obj : m_RenderGroup[RENDER_NONALPHA])
    {
        obj->Render_GameObject();
    }
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& graphicDev)
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

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& graphicDev)
{
    // TODO 석호: 일단 단순 구현해서, 나중에 가능하면 카메라 컴포넌트가 적은 부하로 관리 가능하게 만들자

    // 직교 투영
    if (m_RenderGroup[RENDER_UI].empty())
        return;

    _matrix prevView;
    _matrix prevProj;

    graphicDev->GetTransform(D3DTS_VIEW, &prevView);
    graphicDev->GetTransform(D3DTS_PROJECTION, &prevProj);

    _matrix uiView;
    _matrix uiProj;

    D3DXMatrixIdentity(&uiView);

    D3DXMatrixOrthoOffCenterLH(
        &uiProj,
        0.f,
        static_cast<_float>(WINCX),
        static_cast<_float>(WINCY),
        0.f,
        0.f,
        1.f);

    graphicDev->SetTransform(D3DTS_VIEW, &uiView);
    graphicDev->SetTransform(D3DTS_PROJECTION, &uiProj);

    for (auto& obj : m_RenderGroup[RENDER_UI])
    {
        obj->Render_GameObject();
    }

    graphicDev->SetTransform(D3DTS_VIEW, &prevView);
    graphicDev->SetTransform(D3DTS_PROJECTION, &prevProj);
}

void CRenderer::Free()
{
    Clear_RenderGroup();
}
