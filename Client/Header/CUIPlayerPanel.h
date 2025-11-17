#pragma once
#include "CUIPanel.h"

class CUIPlayerPanel : public CUIPanel
{
	
public:
	explicit CUIPlayerPanel();
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
};  

