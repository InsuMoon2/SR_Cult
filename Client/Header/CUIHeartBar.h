#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

class CUIHeartBar : public CUI
{
public:
    explicit  CUIHeartBar(DEVICE graphicDev);
    explicit  CUIHeartBar(const CUIHeartBar& rhs);
    virtual ~CUIHeartBar();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int    Update_GameObject(const _float& timeDelta) override;
    virtual void    LateUpdate_GameObject(const _float& timeDelta);

    virtual void Render_GameObject();
    HRESULT Add_Component();

public:
    static CUIHeartBar* Create(DEVICE graphicDev);


private:
    CRcTex* m_BufferCom;
    CTexture* m_TextureCom;
    Engine::CTransform* m_TransformCom;


protected:
    void Free() override;
};
END
