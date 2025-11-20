#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CComponent;
class CCollider;

/// @brief 다양한 컴포넌트를 조합해 게임 내 오브젝트를 구현하는 클래스
/// @details
/// - 여러 컴포넌트를 담고, 생명 주기(Ready/Update/Render)를 순차적으로 실행한다.
/// - 외부 노출은 Get_Component()만 제공하여, 이를 통해서만 각 기능에 접근해야 한다
class ENGINE_DLL CGameObject : public CBase
{
protected:
    explicit CGameObject(DEVICE graphicDev);
    explicit CGameObject(const CGameObject& rhs);
    ~CGameObject() override;

public:
    CComponent* Get_Component(COMPONENTID componentID, COMPONENTTYPE componentType);

public:
    virtual HRESULT Ready_GameObject();
    virtual _int    Update_GameObject(const _float& timeDelta);
    virtual void    LateUpdate_GameObject(const _float& timeDelta);
    virtual void    Render_GameObject() PURE;

    // TODO 인수: 순수 가상함수로 바꿀지?
    virtual void OnBeginOverlap(CCollider* self, CCollider* other) {}
    virtual void OnEndOverlap(CCollider* self, CCollider* other) {}

protected:
    map<COMPONENTTYPE, CComponent*> m_Components[ID_END];

    DEVICE m_GraphicDev;

private:
    CComponent* Find_Component(COMPONENTID componentID, COMPONENTTYPE componentType);

protected:
    void Free() override;
};

END
