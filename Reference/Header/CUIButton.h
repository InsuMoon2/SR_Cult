#pragma once
#include "CUI.h"

BEGIN(Engine)
class ENGINE_DLL CUIButton : public CUI
{
protected:
    explicit CUIButton(DEVICE graphicDev);
    explicit CUIButton(const CUIButton& rhs);
    ~CUIButton() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;
    HRESULT Add_Component();

protected:
    bool m_bHovered = false;
    bool m_bClicked = false;
    RECT m_tRect;
public:
    static CUIButton* Create(DEVICE graphicDev);

    void        Set_TextureIndex(int textureIndex) { m_textureIndex = textureIndex; }
    CTransform* Get_Transform() const { return m_TransformCom; }

private:
    static int m_nextID;
    int        m_textureIndex = 0;

private:
    void Free() override;


};

END
