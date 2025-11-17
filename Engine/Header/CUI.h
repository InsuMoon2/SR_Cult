#pragma once
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI : public CGameObject
{
public:
	explicit CUI(DEVICE pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int    Update_GameObject(const _float& timeDelta);
    virtual void    LateUpdate_GameObject(const _float& timeDelta);
    virtual void    Render_GameObject();

protected:
	DEVICE			m_GraphicDev;

protected:
	virtual void				Free();
};
END
