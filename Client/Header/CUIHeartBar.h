#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

class CUIHeartBar : public CUI
{
public:
    explicit  CUIHeartBar();
    explicit  CUIHeartBar(DEVICE pGraphicDev);
    explicit  CUIHeartBar(const CUIHeartBar& rhs);
    virtual ~CUIHeartBar();

public:
    virtual HRESULT Ready_GameObject();
    virtual void Render_GameObject();
    HRESULT Add_Component();

public:
    static CUIHeartBar* Create(DEVICE graphicDev);


private:
    CRcTex* m_BufferCom;
    CTexture* m_TextureCom;

protected:
    void Free() override;
};
END
