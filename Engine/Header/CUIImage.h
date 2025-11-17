#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

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
    HRESULT Add_Component();

public:
    static CUIImage* Create(DEVICE graphicDev);


private:
    CRcTex* m_BufferCom;
    CTexture* m_TextureCom;

protected:
    void Free() override;
};
END
