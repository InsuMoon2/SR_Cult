#include "pch.h"
#include "CCreateHelper.h"
#include "CUIHeartBar.h"
#include "CRcTex.h"
#include "CTexture.h"

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
void CUIHeartBar::Render_GameObject()
{
    m_TextureCom->Set_Texture(0);
    m_BufferCom->Render_Buffer();

}

HRESULT CUIHeartBar::Add_Component()
{
  
    // buffer
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom });

    // texture
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_HEART);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_HEART, m_TextureCom });

    return S_OK;

}

CUIHeartBar* CUIHeartBar::Create(DEVICE graphicDev)
{
    auto image = new CUIHeartBar(graphicDev);
    if (FAILED(image->Ready_GameObject()))
     {
        Safe_Release(image);
        MSG_BOX("UIImage Created Failed");
        return nullptr;
     }


    return image;
}

void CUIHeartBar::Free()
{
    Safe_Release(m_BufferCom);
    Safe_Release(m_TextureCom);

    CUI::Free();
}
