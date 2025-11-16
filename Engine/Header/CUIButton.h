#pragma once
#include "CUI.h"

BEGIN(Engine)
class ENGINE_DLL CUIButton : public CUI
{
protected:
	explicit CUIButton();
	explicit CUIButton(DEVICE pGraphicDev);
	explicit CUIButton(const CUIButton& rhs);
	virtual ~CUIButton();


public:
	virtual _int Update_UI(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_UI() {}

};
END