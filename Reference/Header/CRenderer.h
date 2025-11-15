#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CRenderer : public CBase
{
    DECLARE_SINGLETON(CRenderer)

private:
    explicit CRenderer();
    ~CRenderer() override;

public:
    void Add_RenderGroup(RENDERID type, CGameObject* gameObject);
    void Render_GameObject(LPDIRECT3DDEVICE9& graphicDev);
    void Clear_RenderGroup();

private:
    void Render_Priority(LPDIRECT3DDEVICE9& graphicDev);
    void Render_NonAlpha(LPDIRECT3DDEVICE9& graphicDev);
    void Render_Alpha(LPDIRECT3DDEVICE9& graphicDev);
    void Render_UI(LPDIRECT3DDEVICE9& graphicDev);

private:
    list<CGameObject*> m_RenderGroup[RENDER_END];

private:
    void Free() override;
};

END
