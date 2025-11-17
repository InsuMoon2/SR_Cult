#include "pch.h"
#include "CTestMonster.h"

#include "CAnimator.h"
#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CRectCollider.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CTransform.h"
#include "CState.h"
#include "CEnumHelper.h"

CTestMonster::CTestMonster(DEVICE graphicDev)
    : CGameObject(graphicDev),
    m_BufferCom(nullptr),
    m_TransformCom(nullptr),
    m_TextureCom(nullptr),
    m_AnimatorCom(nullptr),
    m_RectColCom(nullptr),
    m_StateCom(nullptr)
{
}

CTestMonster::CTestMonster(const CTestMonster& rhs)
    : CGameObject(rhs),
    m_BufferCom(nullptr),
    m_TransformCom(nullptr),
    m_TextureCom(nullptr),
    m_AnimatorCom(nullptr),
    m_RectColCom(nullptr),
    m_StateCom(nullptr)
{
}

CTestMonster::~CTestMonster()
{
}

HRESULT CTestMonster::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    Animation_Setting();

    return S_OK;
}

_int CTestMonster::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return exit;
}

void CTestMonster::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CTestMonster::Render_GameObject()
{
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    Render_Setting();

    m_BufferCom->Render_Buffer();

    Render_Reset();

    m_RectColCom->Render(); // Render Reset 이후 호출해야함
}

void CTestMonster::Render_Setting()
{
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_GraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_GraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_GraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_GraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_GraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_GraphicDev->SetRenderState(D3DRS_ALPHAREF, 0);
}

void CTestMonster::Render_Reset()
{
    m_GraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_GraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTestMonster::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnBeginOverlap(self, other);

}

void CTestMonster::OnEndOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnEndOverlap(self, other);

}

HRESULT CTestMonster::Add_Component()
{
    // buffer
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom });

    // transform
    m_TransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_TransformCom });

    // texture
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_MONSTER);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_MONSTER, m_TextureCom });

    // Animator
    m_AnimatorCom = CreateProtoComponent<CAnimator>(this, COMPONENTTYPE::ANIMATOR);
    NULL_CHECK_RETURN(m_AnimatorCom, E_FAIL);

    m_AnimatorCom->Set_TextureType(COMPONENTTYPE::TEX_PLAYER);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::ANIMATOR, m_AnimatorCom });

    // RectCol Componet
    m_RectColCom = CreateProtoComponent<CRectCollider>(this, COMPONENTTYPE::RECT_COLL);
    NULL_CHECK_RETURN(m_RectColCom, E_FAIL);
    m_RectColCom->Set_Size(_vec2(2.f, 2.f));

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::RECT_COLL, m_RectColCom });

    m_StateCom = CreateProtoComponent<CState>(this, COMPONENTTYPE::STATE);
    NULL_CHECK_RETURN(m_StateCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::STATE, m_StateCom });

    return S_OK;
}

void CTestMonster::Animation_Setting()
{

}

CTestMonster* CTestMonster::Create(DEVICE graphicDev)
{
    auto monster = new CTestMonster(graphicDev);

    if (FAILED(monster->Ready_GameObject()))
    {
        MSG_BOX("Monster Create Failed");
        Safe_Release(monster);
        return nullptr;
    }

    return monster;
}

void CTestMonster::Free()
{
    CGameObject::Free();
}
