#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CCollider;

// 프레임마다 모든 콜라이더를 탐색해서
// BeginOverlap, EndOverlap을 GameObject에 전달

class ENGINE_DLL CCollisionManager : public CBase
{
    DECLARE_SINGLETON(CCollisionManager)

private:
    explicit CCollisionManager();
    virtual ~CCollisionManager();

public:
    void Ready_Collision();
    void Update();

    void AddCollider(CCollider* collider);
    void RemoveCollider(CCollider* collider);

private:
    void Free() override;

private:
    vector<CCollider*> m_Colliders;

};

END
