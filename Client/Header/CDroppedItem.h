#pragma once
#include "CGameObject.h"

namespace Engine
{
    class CTransform;
    class CRectCollider;
    class CRcTex;
}

class CDroppedItem :
    public CGameObject
{
    explicit CDroppedItem(DEVICE graphicDev);
    explicit CDroppedItem(const CDroppedItem& rhs);
    virtual ~CDroppedItem() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

    void OnBeginOverlap(CCollider* self, CCollider* other) override;
    void OnEndOverlap(CCollider* self, CCollider* other) override;

private:
    HRESULT Add_Component();

private:
    Engine::CRcTex* m_BufferCol;
    Engine::CTransform* m_TransformCom;
    Engine::CRectCollider* m_RectColCom;
    
public:
    static CDroppedItem* Create(DEVICE graphicDev);

private:
    void Free() override;
};

