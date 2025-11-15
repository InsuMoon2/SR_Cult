#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(COMPONENTID eID, COMPONENTTYPE eComponentType);

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() = 0;

    // 순수 가상함수로 바꿀지?
    virtual         void        OnBeginOverlap(CCollider* self, CCollider* other) {};
    virtual         void        OnEndOverlap(CCollider* self, CCollider* other) {};

protected:
	map<COMPONENTTYPE, CComponent*>			m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;


private:
	CComponent* Find_Component(COMPONENTID eID, COMPONENTTYPE eComponentType);

protected:
	virtual		void		Free();
};

END
