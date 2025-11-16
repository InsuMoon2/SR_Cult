#pragma once
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI : public CGameObject
{
public:
	explicit CUI();
	explicit CUI(DEVICE pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	virtual HRESULT Ready_UI() { return S_OK; }
	virtual _int Update_UI(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_UI() {}
	virtual void Render_UI() {}
private:
	DEVICE			m_pGraphicDev;

protected:
	virtual void				Free();
};
END