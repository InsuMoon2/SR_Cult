#pragma once
#include "CGameObject.h"
#include "ItemInstance.h"
namespace Engine
{
    class CTransform;
    class CRectCollider;
    class CRcTex;
    class CTexture;
}

class CDroppedItem :
    public CGameObject
{
    explicit CDroppedItem(DEVICE graphicDev, ItemInstance itemInst);
    explicit CDroppedItem(const CDroppedItem& rhs);
    virtual ~CDroppedItem() override;

public:
    HRESULT Ready_GameObject() override;
    HRESULT Ready_GameObject(_vec3 pos) ;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

    void OnBeginOverlap(CCollider* self, CCollider* other) override;
    void OnEndOverlap(CCollider* self, CCollider* other) override;

private:
    HRESULT Add_Component();

private:
    Engine::CRcTex* m_BufferCom;
    Engine::CTransform* m_TransformCom;
    Engine::CTexture* m_TextureCom;
    Engine::CRectCollider* m_RectColCom;
    
public:
    static CDroppedItem* Create(DEVICE graphicDev, ItemInstance itemInst, _vec3 pos);

private:
    ItemInstance m_itemInst;
private:
    void Free() override;
};

