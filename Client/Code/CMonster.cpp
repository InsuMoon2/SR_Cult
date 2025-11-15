#include "pch.h"
#include "CMonster.h"

#include <CCreateHelper.h>

#include "CProtoMgr.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CRectCollider.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CMonster& rhs)
	:CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	/*Engine::CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>
		(Engine::CManagement::GetInstance()->Get_Component
		(ID_DYNAMIC, L"GameLogic_Layer", L"Player", L"Com_Transform"));

	if (nullptr == pPlayerTransformCom)
		return;*/

    CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(
        CManagement::GetInstance()->Get_Component(ID_DYNAMIC, LAYERTYPE::GameLogic, OBJTYPE::Player, COMPONENTTYPE::Transform));

    NULL_CHECK(pPlayerTransformCom);

	_vec3	vPlayerPos{};
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, fTimeDelta, 5.f);

}

void CMonster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCol->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_RectColCom->Render();
}

void CMonster::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnBeginOverlap(self, other);

}

void CMonster::OnEndOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnEndOverlap(self, other);

}

HRESULT CMonster::Add_Component()
{
	// buffer
    m_pBufferCol = CreateProtoComponent<CTriCol>(this, COMPONENTTYPE::TriCol);
    NULL_CHECK_RETURN(m_pBufferCol, E_FAIL);

	m_mapComponent[ID_STATIC].insert({ COMPONENTTYPE::TriCol, m_pBufferCol });

	// transform
    m_pTransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::Transform);
    NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

    m_mapComponent[ID_DYNAMIC].insert({ COMPONENTTYPE::Transform, m_pTransformCom });

    // RectColCom
    m_RectColCom = CreateProtoComponent<CRectCollider>(this, COMPONENTTYPE::RectCol);
    NULL_CHECK_RETURN(m_RectColCom, E_FAIL);
    m_RectColCom->Set_Size(_vec2(2.f, 2.f));

    m_mapComponent[ID_DYNAMIC].insert({ COMPONENTTYPE::RectCol, m_RectColCom });

	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* Monster = new CMonster(pGraphicDev);

	if (FAILED(Monster->Ready_GameObject()))
	{
		MSG_BOX("Monster Create Failed");
		Safe_Release(Monster);
		return nullptr;
	}

	return Monster;
}

void CMonster::Free()
{
	CGameObject::Free();
}
