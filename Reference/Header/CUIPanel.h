#pragma once
#include "CUI.h"

BEGIN(Engine)
class ENGINE_DLL CUIPanel : public CUI
{
public:
	explicit  CUIPanel(DEVICE graphicDev);
	explicit  CUIPanel(const CUIPanel& rhs);
	virtual ~CUIPanel();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int    Update_GameObject(const _float& timeDelta);
    virtual void    LateUpdate_GameObject(const _float& timeDelta);
    virtual void    Render_GameObject();


public:
	virtual void AddChild(CUI* ui);

protected:
	virtual void				Free();

private:
	vector<CUI*> m_Children;
};
END
