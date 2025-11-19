#include "pch.h"
#include "CMainBG.h"

#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CRenderer.h"
#include "CTexture.h"

CMainBG::CMainBG(DEVICE graphicDev)
    : CGameObject(graphicDev), m_BufferCom(nullptr), m_TextureCom(nullptr)
{ }

CMainBG::CMainBG(const CMainBG& rhs)
    : CGameObject(rhs)
{ }

CMainBG::~CMainBG()
{ }

HRESULT CMainBG::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CMainBG::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

    return exit;
}

void CMainBG::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CMainBG::Render_GameObject()
{
    m_TextureCom->Set_Texture(0);

    m_BufferCom->Render_Buffer();
}

HRESULT CMainBG::Add_Component()
{
    // buffer
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);
   
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom });
   
    // texture
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_MAINBG);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);
   
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_MAINBG, m_TextureCom });

    return S_OK;
}

CMainBG* CMainBG::Create(DEVICE pGraphicDev)
{
    auto backGround = new CMainBG(pGraphicDev);

    if (FAILED(backGround->Ready_GameObject()))
    {
        MSG_BOX("MainBG Create Failed");
        Safe_Release(backGround);
        return nullptr;
    }

    return backGround;
}

void CMainBG::Free()
{
    CGameObject::Free();
}
