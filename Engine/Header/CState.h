#pragma once

#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CAnimator;

class ENGINE_DLL CState : public CComponent
{
private:
    explicit CState(DEVICE GraphicDev);
    explicit CState(const CState& rhs);
    ~CState() override;

public:
    HRESULT Ready_StateComponent(DEVICE GraphicDev);
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    void Update_Animation();

    void Change_State(ACTORSTATE State);
    void Change_Dir(ACTORDIR Dir);

    ACTORSTATE Get_State() const { return m_State; }
    ACTORDIR   Get_Dir() const { return m_Dir; }

    void Set_AnimInfo(ACTORSTATE PlayerState, const wstring& Key, ANIMSTATE AnimState);

    static CState* Create(DEVICE GraphicDev);
    CComponent*    Clone() override;

protected:
    void Free() override;

private:
    ACTORSTATE m_State;
    ACTORSTATE m_PrevState;

    // PrevDir도 필요하면 구현하기
    ACTORDIR m_Dir;

    CAnimator* m_Animator;

    map<ACTORSTATE, wstring>   m_PlayerStateMap;
    map<ACTORSTATE, ANIMSTATE> m_AnimStateMap;
};

END
