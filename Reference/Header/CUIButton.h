#pragma once
#include "CUI.h"

BEGIN(Engine)
class ENGINE_DLL CUIButton : public CUI
{
protected:
    explicit CUIButton();
    explicit CUIButton(DEVICE graphicDev);
    explicit CUIButton(const CUIButton& rhs);
    ~CUIButton() override;

public:
    virtual _int Update_UI(const _float& timeDelta) { return 0; }
    virtual void LateUpdate_UI() {}
};

END
