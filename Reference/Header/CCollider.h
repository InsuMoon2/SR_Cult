#pragma once

#include "CComponent.h"

BEGIN(Engine)

class CRectCollider;
class CSphereCollider;

class ENGINE_DLL CCollider : public CComponent
{
protected:
    explicit CCollider(DEVICE GraphicDev, COLLIDERTYPE eColliderType);
    explicit CCollider(const CCollider& rhs);
    virtual  ~CCollider() override;

public:
    HRESULT Ready_Collider();
    virtual _int Update_Component(const _float& fTimeDelta);
    virtual void LateUpdate_Component() override;
    virtual void Render();

    virtual bool CheckCollision(CCollider* other);

    COLLIDERTYPE GetColliderType() const { return m_ColliderType; }

    void Set_ShowDebug(bool Show) { m_ShowDebug = Show; }
    bool Get_ShowDebug() const { return m_ShowDebug; }

    // [충돌 상태 관리] - CollisionManager에서 사용
    bool IsOverlapping(CCollider* other);
    void AddOverlap(CCollider* other);      // 충돌시작
    void RemoveOverlap(CCollider* other);   // 충돌 끝

public:
    static bool CheckCollisionBox2Box(CRectCollider* b1, CRectCollider* b2);
    static bool CheckCollisionSphere2Box(CSphereCollider* s1, CRectCollider* b2);
    static bool CheckCollisionSphere2Sphere(CSphereCollider* s1, CSphereCollider* s2);

protected:
    COLLIDERTYPE m_ColliderType; 
    bool m_ShowDebug = true;      // 디버그 렌더링 여부

private:
    // 현재 프레임 기준으로 겹쳐있는(충돌중인) 다른 Collider들
    unordered_set<CCollider*> m_ColliderSet;

};

END

