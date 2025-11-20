#pragma once
#include "CUIPanel.h"
#include "CPlayer.h"

class CUIXpPanel : public CUIPanel
{
public:
    explicit CUIXpPanel(DEVICE graphicDev);
    explicit CUIXpPanel(const CUIXpPanel& rhs);
    ~CUIXpPanel() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

public:
    static CUIXpPanel* Create(DEVICE graphicDev);

protected:
    void Free() override;


};

