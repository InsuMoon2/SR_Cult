#pragma once
#include "CUI.h"

BEGIN(Engine)
class ENGINE_DLL CUIPanel : public CUI
{
public:
	explicit  CUIPanel();
	explicit  CUIPanel(DEVICE pGraphicDev);
	explicit  CUIPanel(const CUIPanel& rhs);
	virtual ~CUIPanel();

public:
	virtual HRESULT Ready_Panel() { return S_OK; }
	virtual _int Update_Panel(const _float& fTimeDelta);
	virtual void LateUpdate_Panel() {}
	virtual void Render_Panel() {};

public:
	virtual void AddChild(CUI* ui);

protected:
	virtual void				Free();

private:
	vector<CUI*> m_Children;
};
END
