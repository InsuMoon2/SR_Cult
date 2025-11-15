#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual _int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() { }

	CGameObject* Get_Owner() { return m_Owner; }
    void Set_Owner(CGameObject* Owner);

    CTransform* Get_Transform() { return m_TransformCom; }

protected:
	DEVICE			m_GraphicDev;
	_bool			m_Clone;

	CGameObject*	m_Owner;
    CTransform*     m_TransformCom;

public:
	virtual CComponent* Clone()	PURE;		// 프로토타입 사용 예

protected:
	virtual void				Free();
};

END
