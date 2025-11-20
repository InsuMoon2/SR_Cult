#pragma once
#include "CUIPanel.h"
#include "CPlayer.h"

class CUIPlayerPanel : public CUIPanel
{
public:
    explicit CUIPlayerPanel(DEVICE graphicDev);
    explicit CUIPlayerPanel(const CUIPlayerPanel& rhs);
    ~CUIPlayerPanel() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

public:
    static CUIPlayerPanel* Create(DEVICE graphicDev);
    void Set_Player(CPlayer* player) { m_Player = player; }

protected:
    void Free() override;

private:
    CPlayer* m_Player;

};
