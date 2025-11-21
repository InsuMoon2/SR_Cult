#include "pch.h"
#include "CUIMp.h"

#include <CCombatStat.h>

#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CTransform.h"

int CUIMp::m_nextID = 0;

CUIMp::CUIMp(DEVICE pGraphicDev) : CUI(pGraphicDev)
{

    m_ID = ++m_nextID;
}

CUIMp::CUIMp(const CUIMp& rhs) : CUI(rhs)
{
    m_ID = ++m_nextID;
}

CUIMp::~CUIMp()
{
}

HRESULT CUIMp::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(_vec3(-550.f, 260.f, 0.f));
    m_TransformCom->Set_Scale(_vec3(54.f, 54.f, 0.f));
    return S_OK;
}

_int CUIMp::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUI::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUIMp::LateUpdate_GameObject(const _float& timeDelta)
{
    CUI::LateUpdate_GameObject(timeDelta);
}

void CUIMp::Render_GameObject()
{
    CUI::Render_GameObject();

    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    m_TextureCom->Set_Texture(0);

    float currMp = m_CombatStatCom->Get_Mp();
    float maxMp = m_CombatStatCom->Get_MaxMp();

    float ratio = currMp / maxMp;
    float vStart = 1.0f - ratio;

    //m_BufferCom->Set_UV(0.f, vStart, 1.f, 1.f);

    m_BufferCom->Render_Buffer();
}

HRESULT CUIMp::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_COLOR);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_COLOR, m_TextureCom });

    return S_OK;
}

CUIMp* CUIMp::Create(DEVICE graphicDev)
{
    auto circle = new CUIMp(graphicDev);
    if (FAILED(circle->Ready_GameObject()))
    {
        Safe_Release(circle);
        MSG_BOX("UIXp Created Failed");
        return nullptr;
    }
    return circle;
}

void CUIMp::Free()
{

    CUI::Free();
}
