#pragma once
#include "CUIPanel.h"

class CUIMainMenuPanel : public CUIPanel
{
	
public:
	explicit CUIMainMenuPanel();
	explicit CUIMainMenuPanel(DEVICE pGraphicDev);
	explicit CUIMainMenuPanel(const CUIMainMenuPanel& rhs);
	virtual ~CUIMainMenuPanel();

public:
	virtual HRESULT Ready_Panel() override;
	virtual _int Update_Panel(const _float& fTimeDelta) override;
	virtual void LateUpdate_Panel() override;
	virtual void Render_Panel() override;

public:
    static CUIMainMenuPanel* Create(DEVICE pGraphicDev);
};

