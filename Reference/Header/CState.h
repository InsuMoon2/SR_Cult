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
    virtual  ~CState();

public:
    HRESULT Ready_StateComponent(DEVICE GraphicDev);
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    void    Change_State(PLAYERSTATE State);
    void    Change_Dir(PLAYERDIR Dir);

    PLAYERSTATE Get_State() const { return m_State; }
    PLAYERDIR   Get_Dir()   const { return m_Dir; }

    void Set_AnimInfo(PLAYERSTATE PlayerState, const wstring& Key, ANIMSTATE AnimState);

    static CState* Create(DEVICE GraphicDev);
    CComponent* Clone() override;

protected:
    void Free() override;

private:
    PLAYERSTATE m_State;
    PLAYERSTATE m_PrevState;

    // PrevDir도 필요하면 구현하기
    PLAYERDIR   m_Dir;

    CAnimator*  m_Animator;

    map<PLAYERSTATE, wstring>       m_PlayerStateMap;
    map<PLAYERSTATE, ANIMSTATE>     m_AnimStateMap;

};

END
