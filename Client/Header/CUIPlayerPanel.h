#pragma once
#include "CUIPanel.h"
#include "CPlayer.h"

class CUIPlayerPanel : public CUIPanel
{
	
public:

	explicit CUIPlayerPanel(DEVICE graphicDev);
	explicit CUIPlayerPanel(const CUIPlayerPanel& rhs);
	virtual ~CUIPlayerPanel();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& timeDelta) override;
	virtual void LateUpdate_GameObject(const _float& timeDelta) override;
	virtual void Render_GameObject() override;

public:
    static CUIPlayerPanel* Create(DEVICE graphicDev);
    void Set_Player(CPlayer* player) { m_player = player; }

protected:
    virtual void				Free();

private:
    CPlayer* m_player;
};  

