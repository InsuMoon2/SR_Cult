#include "pch.h"
#include "CCreateHelper.h"
#include "CUIHeartBar.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CAnimator.h"

CUIHeartBar::CUIHeartBar(DEVICE graphicDev) : CUI(graphicDev), m_BufferCom(nullptr), m_TextureCom(nullptr)
{
}

CUIHeartBar::CUIHeartBar(const CUIHeartBar& rhs) :CUI(rhs), m_BufferCom(nullptr), m_TextureCom(nullptr)
{
}

CUIHeartBar::~CUIHeartBar()
{
}

HRESULT CUIHeartBar::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CUIHeartBar::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUI::Update_GameObject(timeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUIHeartBar::LateUpdate_GameObject(const _float& timeDelta)
{
    return;
}

void CUIHeartBar::Render_GameObject()
{
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    cout << "Heart Pos x : " << m_TransformCom->Get_Pos().x << endl;
    cout << "Heart Pos y : " << m_TransformCom->Get_Pos().y << endl;
    cout << "Heart Pos z : " << m_TransformCom->Get_Pos().z << endl;

    m_TextureCom->Set_Texture(2);
    m_BufferCom->Render_Buffer();

}

HRESULT CUIHeartBar::Add_Component()
{
  
    // buffer
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);
  
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom });

    // transform
    m_TransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_TransformCom });

    // texture
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_HEART);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_HEART, m_TextureCom });


    return S_OK;

}

CUIHeartBar* CUIHeartBar::Create(DEVICE graphicDev)
{
    auto heartbar = new CUIHeartBar(graphicDev);
    if (FAILED(heartbar->Ready_GameObject()))
     {
        Safe_Release(heartbar);
        MSG_BOX("UIHeartBar Created Failed");
        return nullptr;
     }


    return heartbar;
}

void CUIHeartBar::Free()
{
    Safe_Release(m_BufferCom);
    Safe_Release(m_TextureCom);

    CUI::Free();
}
