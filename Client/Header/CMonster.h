#pragma once

#include "CGameObject.h"
#include "CTriCol.h"
#include "CTransform.h"

BEGIN(Engine)
class CRectCollider;
END

class CMonster : public CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void OnBeginOverlap(CCollider* self, CCollider* other) override;
	void OnEndOverlap(CCollider* self, CCollider* other) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CTriCol*	m_pBufferCol;
	Engine::CTransform* m_pTransformCom;

    Engine::CRectCollider* m_RectColCom;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

