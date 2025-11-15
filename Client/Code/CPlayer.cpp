#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CAnimator.h"
#include "CCreateHelper.h"
#include "CRectCollider.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{

}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->m_vScale = { 2.f, 1.f, 1.f };

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


	return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    Render_Setting();
	
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

    Render_Reset();

    m_RectColCom->Render(); // Render Reset 이후 호출해야함
}

void CPlayer::Render_Setting()
{
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0);
}

void CPlayer::Render_Reset()
{
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnBeginOverlap(self, other);

    cout << "Player Hit" << endl;
}

void CPlayer::OnEndOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnEndOverlap(self, other);
}

HRESULT CPlayer::Add_Component()
{
	// buffer
    //m_pBufferCom = CreateProtoComponent<CRcTex>(this, L"Proto_RcTex");
    m_pBufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RcTex);
    NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);

	m_mapComponent[ID_STATIC].insert({ COMPONENTTYPE::RcTex, m_pBufferCom });

	// transform
    //m_pTransformCom = CreateProtoComponent<CTransform>(this, L"Proto_Transform");
    m_pTransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::Transform);
    NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

	m_mapComponent[ID_DYNAMIC].insert({ COMPONENTTYPE::Transform, m_pTransformCom });

	// texture
    m_pTextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::Texture);
    NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);

	m_mapComponent[ID_STATIC].insert({ COMPONENTTYPE::Texture, m_pTextureCom });

    m_AnimatorCom = CreateProtoComponent<CAnimator>(this, COMPONENTTYPE::Animator);
    NULL_CHECK_RETURN(m_AnimatorCom, E_FAIL);

    m_AnimatorCom->Ready_Animator();

    m_mapComponent[ID_DYNAMIC].insert({ COMPONENTTYPE::Animator, m_AnimatorCom });

    // TestPlayer
	//m_Animator->Create_LineAnimation(L"Idle", 24, 12, 9, 0.1f, ANIMSTATE::Loop);
	//m_Animator->Create_LineAnimation(L"Run", 24, 12, 1, 0.1f, ANIMSTATE::Loop);

    // Lamb Idle
    m_AnimatorCom->Create_Animation(L"Idle", 150, 1, 1, 0.02f, ANIMSTATE::Loop);

    // 시작 애니메이션
    m_AnimatorCom->Play_Animation(L"Idle");

    // RectCol Componet
    m_RectColCom = CreateProtoComponent<CRectCollider>(this, COMPONENTTYPE::RectCol);
    NULL_CHECK_RETURN(m_RectColCom, E_FAIL);
    m_RectColCom->Set_Size(_vec2(2.f, 2.f));

    m_mapComponent[ID_DYNAMIC].insert({ COMPONENTTYPE::RectCol, m_RectColCom });

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    const _float fSpeed = 10.f;

    _vec3 vDir;
    m_pTransformCom->Get_Info(INFO_LOOK, &vDir);

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fSpeed);
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, -fSpeed);
    }

    m_pTransformCom->Get_Info(INFO_RIGHT, &vDir);
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, fSpeed);
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        D3DXVec3Normalize(&vDir, &vDir);
        m_pTransformCom->Move_Pos(&vDir, fTimeDelta, -fSpeed);
    }

}
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pPlayer = new CPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		MSG_BOX("pPlayer Create Failed");
		Safe_Release(pPlayer);
		return nullptr;
	}

	return pPlayer;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
