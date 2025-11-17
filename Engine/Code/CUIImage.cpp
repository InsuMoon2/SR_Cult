#include "CCreateHelper.h"
#include "CUIImage.h"
#include "CRcTex.h"
#include "CTexture.h"

CUIImage::CUIImage()
{
}

CUIImage::CUIImage(DEVICE pGraphicDev)
{
}

CUIImage::CUIImage(const CUIImage& rhs)
{
}

CUIImage::~CUIImage()
{
}

HRESULT CUIImage::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}
void CUIImage::Render_GameObject()
{
    m_TextureCom->Set_Texture(0);
    m_BufferCom->Render_Buffer();

}

HRESULT CUIImage::Add_Component()
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

CUIImage* CUIImage::Create(DEVICE graphicDev)
{
    auto image = new CUIImage(graphicDev);
    if (FAILED(image->Ready_GameObject()))
     {
        Safe_Release(image);
        MSG_BOX("UIImage Created Failed");
        return nullptr;
     }


    return image;
}

void CUIImage::Free()
{
}
