#pragma once
#include "CUIPanel.h"

class CUIMainMenuPanel : public CUIPanel
{
	
public:
	explicit CUIMainMenuPanel();
	explicit CUIMainMenuPanel(DEVICE graphicDev);
	explicit CUIMainMenuPanel(const CUIMainMenuPanel& rhs);
	virtual ~CUIMainMenuPanel();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& timeDelta) override;
	virtual void LateUpdate_GameObject(const _float& timeDelta) override;
	virtual void Render_GameObject() override;

public:
    static CUIMainMenuPanel* Create(DEVICE graphicDev);
};

