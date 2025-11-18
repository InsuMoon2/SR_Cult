#include"pch.h"
#include"CDroppedItem.h"

CDroppedItem::CDroppedItem(DEVICE graphicDev) : CGameObject(graphicDev)
{
}

CDroppedItem::CDroppedItem(const CDroppedItem& rhs) : CGameObject(rhs)
{
}

CDroppedItem::~CDroppedItem()
{
}

HRESULT CDroppedItem::Ready_GameObject()
{
    return E_NOTIMPL;
}

_int CDroppedItem::Update_GameObject(const _float& timeDelta)
{
    return _int();
}

void CDroppedItem::LateUpdate_GameObject(const _float& timeDelta)
{
}

void CDroppedItem::Render_GameObject()
{
}

void CDroppedItem::OnBeginOverlap(CCollider* self, CCollider* other)
{
}

void CDroppedItem::OnEndOverlap(CCollider* self, CCollider* other)
{
}

HRESULT CDroppedItem::Add_Component()
{
    return E_NOTIMPL;
}

CDroppedItem* CDroppedItem::Create(DEVICE graphicDev)
{
    return nullptr;
}

void CDroppedItem::Free()
{
}
