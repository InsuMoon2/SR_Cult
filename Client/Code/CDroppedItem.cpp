#include"pch.h"
#include"CDroppedItem.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CTransform.h"
#include "CRectCollider.h"
#include "CCreateHelper.h"
#include "CRenderer.h"
#include "CProtoMgr.h"
#include "CItemDB.h"
#include "ItemData.h"

CDroppedItem::CDroppedItem(DEVICE graphicDev, ItemInstance itemInst)
    : CGameObject(graphicDev), m_itemInst(itemInst),
      m_BufferCom(nullptr),m_RectColCom(nullptr), m_TextureCom(nullptr), m_TransformCom(nullptr)
{
}

CDroppedItem::CDroppedItem(const CDroppedItem& rhs) : CGameObject(rhs), m_itemInst(rhs.m_itemInst),
m_BufferCom(rhs.m_BufferCom), m_RectColCom(rhs.m_RectColCom), m_TextureCom(rhs.m_TextureCom), m_TransformCom(rhs.m_TransformCom)
{
}

CDroppedItem::~CDroppedItem()
{
}

HRESULT CDroppedItem::Ready_GameObject()
{
    // 이건 오버라이드 용으로 만들어 논거임 의미 없을듯
    return S_OK;
}

HRESULT CDroppedItem::Ready_GameObject(_vec3 pos)
{
    m_index = CItemDB::GetInstance()->GetIndexById(m_itemInst.itemId);

    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(pos);

    return S_OK;
}

_int CDroppedItem::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return exit;
}

void CDroppedItem::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CDroppedItem::Render_GameObject()
{
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());
    
    m_TextureCom->Set_Texture(m_index);

    m_BufferCom->Render_Buffer();

}

void CDroppedItem::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnBeginOverlap(self, other);
    // 여기에서 드롭 시스템 호출
    cout << "item Hit" << endl;
}

void CDroppedItem::OnEndOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnEndOverlap(self, other);

}

HRESULT CDroppedItem::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_ITEM);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_ITEM, m_TextureCom });


    // RectCol Componet
    m_RectColCom = CreateProtoComponent<CRectCollider>(this, COMPONENTTYPE::RECT_COLL);
    NULL_CHECK_RETURN(m_RectColCom, E_FAIL);
    m_RectColCom->Set_Size(_vec2(2.f, 2.f));

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::RECT_COLL, m_RectColCom });

    return S_OK;
}

void CDroppedItem::PopDrop(float range, float height)
{
    // 추후 구현예정
}

CDroppedItem* CDroppedItem::Create(DEVICE graphicDev, ItemInstance itemInst, _vec3 pos)
{
    auto DroppedItem = new CDroppedItem(graphicDev, itemInst);

    if (FAILED(DroppedItem->Ready_GameObject(pos)))
    {
        MSG_BOX("pPlayer Create Failed");
        Safe_Release(DroppedItem);
        return nullptr;
    }

    return DroppedItem;
}

void CDroppedItem::Free()
{
    CGameObject::Free();
}
