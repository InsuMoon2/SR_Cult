#include "pch.h"
#include "CUIInventoryCell.h"
#include "CEnumHelper.h"
#include "CCreateHelper.h"
#include "CTransform.h"
#include "CRcTex.h"
#include "CTexture.h"

CUIInventoryCell::CUIInventoryCell(DEVICE pGraphicDev): CUI(pGraphicDev)
 {
     
 }

 CUIInventoryCell::CUIInventoryCell(const CUIInventoryCell& rhs): CUI(rhs)
 {
     
 }

 CUIInventoryCell::~CUIInventoryCell()
 {
     
 } 

 void CUIInventoryCell::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CUI::OnBeginOverlap(self, other);
}

 void CUIInventoryCell::OnEndOverlap(CCollider* self, CCollider* other)
{
    CUI::OnEndOverlap(self, other);
}

 void CUIInventoryCell::Render_Editor()
{
    CUI::Render_Editor();
}

 HRESULT CUIInventoryCell::Ready_GameObject()
{
    return S_OK;
}

 _int CUIInventoryCell::Update_GameObject(const _float& timeDelta)
{
     int exit = CUI::Update_GameObject(timeDelta);

    return exit;
}

 void CUIInventoryCell::LateUpdate_GameObject(const _float& timeDelta)
{
    CUI::LateUpdate_GameObject(timeDelta);
}

 void CUIInventoryCell::Render_GameObject()
{
    CUI::Render_GameObject();
}

HRESULT CUIInventoryCell::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_CELL);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_HEART, m_TextureCom });

    return S_OK;
 }

void CUIInventoryCell::Free()
{
    CUI::Free();
}
