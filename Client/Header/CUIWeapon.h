#pragma once
#include "CUI.h"

class CUIWeapon : public CUI
{
public:

    explicit CUIWeapon(DEVICE pGraphicDev);
    explicit CUIWeapon(const CUIWeapon& rhs);
    ~CUIWeapon() override;

    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& timeDelta) override;
    void LateUpdate_GameObject(const _float& timeDelta) override;
    void Render_GameObject() override;

    HRESULT Add_Component();

public:
    static CUIWeapon* Create(DEVICE graphicDev);

private:
    static int         m_nextID;

protected:
    void Free() override;

};

