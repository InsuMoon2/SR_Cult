#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CUI : public CGameObject
{
public:
    explicit CUI(DEVICE pGraphicDev);
    explicit CUI(const CUI& rhs);
    ~CUI() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

protected:
    void Free() override;
};

END
