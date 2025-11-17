#include "CUIImage.h"

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
