#pragma once
#include "CUI.h"

BEGIN(Engine)
class CUIImage : public CUI
{
public:
    explicit  CUIImage();
    explicit  CUIImage(DEVICE pGraphicDev);
    explicit  CUIImage(const CUIImage& rhs);
    virtual ~CUIImage();

public:
    virtual HRESULT Ready_GameObject();
    virtual void Render_GameObject();

public:
    static CUIImage* Create(DEVICE graphicDev);

};
END
