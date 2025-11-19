#include "CState.h"
#include "CAnimator.h"
#include "CGameObject.h"

CState::CState(DEVICE GraphicDev)
    : CComponent(GraphicDev)
      , m_State(ACTORSTATE::IDLE)
      , m_PrevState(ACTORSTATE::PLAYERSTATE_END)
      , m_Dir(ACTORDIR::LEFT)
      , m_Animator(nullptr)
{}

CState::CState(const CState& rhs)
    : CComponent(rhs.m_GraphicDev)
      /*, m_State(rhs.m_State)
      , m_Dir(rhs.m_Dir)
      , m_PrevState(rhs.m_PrevState)
      , m_Animator(rhs.m_Animator)*/
      , m_State(ACTORSTATE::IDLE)
      , m_PrevState(ACTORSTATE::PLAYERSTATE_END)
      , m_Dir(ACTORDIR::LEFT)
      , m_Animator(nullptr)
{}

CState::~CState()
{
    Free();
}

HRESULT CState::Ready_StateComponent(DEVICE GraphicDev)
{
    m_State     = ACTORSTATE::IDLE;
    m_PrevState = ACTORSTATE::PLAYERSTATE_END;

    m_Dir = ACTORDIR::LEFT;

    return S_OK;
}

_int CState::Update_Component(const _float& timeDelta)
{
    _int exit = CComponent::Update_Component(timeDelta);

    Update_Animation();

    return exit;
}

void CState::LateUpdate_Component()
{
    CComponent::LateUpdate_Component();
}

void CState::Update_Animation()
{
    // Animator를 안 들고 있으면, Owner에서 한번 탐색
    if (m_Animator == nullptr && m_Owner != nullptr)
    {
        m_Animator = dynamic_cast<CAnimator*>(
            m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::ANIMATOR));

        // Animator가 없으면 그냥 넘어감 Null 체크는 아래에서 진행
    }

    if (m_Animator != nullptr && m_State != m_PrevState)
    {
        auto iter = m_PlayerStateMap.find(m_State);
        if (iter != m_PlayerStateMap.end())
        {
            ANIMSTATE animState = ANIMSTATE::LOOP; // 기본 LOOP 로 세팅

            auto animIter = m_AnimStateMap.find(m_State);

            if (animIter != m_AnimStateMap.end())
                animState = animIter->second;

            m_Animator->Play_Animation(iter->second, animState);
        }

        m_PrevState = m_State;
    }
}

void CState::Change_State(ACTORSTATE State)
{
    if (m_State == State)
        return;

    m_State = State;
}

void CState::Change_Dir(ACTORDIR Dir)
{
    if (m_Dir == Dir)
        return;

    m_Dir = Dir;
}

void CState::Set_AnimInfo(ACTORSTATE PlayerState, const wstring& Key, ANIMSTATE AnimState)
{
    m_PlayerStateMap[PlayerState] = Key;
    m_AnimStateMap[PlayerState]   = AnimState;
}

CState* CState::Create(DEVICE GraphicDev)
{
    CState* state = new CState(GraphicDev);

    if (FAILED(state->Ready_StateComponent(GraphicDev)))
    {
        Safe_Release(state);
        MSG_BOX("StateCom Create Failed");
        return nullptr;
    }

    return state;
}

CComponent* CState::Clone()
{
    return new CState(*this);
}

void CState::Free()
{
    CComponent::Free();
}
