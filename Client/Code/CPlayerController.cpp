#include "pch.h"
#include "CPlayerController.h"

#include "CCombatStat.h"
#include "CDInputMgr.h"
#include "CGameObject.h"
#include "CState.h"
#include "CTransform.h"

CPlayerController::CPlayerController(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_OwnerStateCom(nullptr),
      m_OwnerCombatStat(nullptr)
{ }

CPlayerController::CPlayerController(const CPlayerController& rhs)
    : CComponent(rhs),
      m_OwnerStateCom(nullptr),
      m_OwnerCombatStat(nullptr)
{ }

CPlayerController::~CPlayerController()
{ }

HRESULT CPlayerController::Ready_PlayerController()
{
    return S_OK;
}

_int CPlayerController::Update_Component(const _float& timeDelta)
{
    Key_Input(timeDelta);

    return 0;
}

void CPlayerController::LateUpdate_Component()
{ }

HRESULT CPlayerController::Init_OwnerComponent()
{
    m_OwnerStateCom =
        dynamic_cast<CState*>(m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::STATE));

    m_OwnerCombatStat =
        dynamic_cast<CCombatStat*>(m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::COMBATSTAT));

    NULL_CHECK_RETURN_MSG(
        m_OwnerStateCom,
        E_FAIL,
        L"CPlayerController::Init_OwnerComponent(): Owner's STATE caching has failed");

    NULL_CHECK_RETURN_MSG(
        m_OwnerCombatStat,
        E_FAIL,
        L"CPlayerController::Init_OwnerComponent(): Owner's COMBATSTAT caching has failed");

    return S_OK;
}

void CPlayerController::Key_Input(const _float& timeDelta)
{
    const auto inputMgr = CDInputMgr::GetInstance();

    // ---------------------------------
    // 1. 방향 입력 벡터 계산
    // ---------------------------------
    _vec3 moveDir = { 0.f, 0.f, 0.f };
    int   axis_X{};
    int   axis_Z{};

    if (inputMgr->Get_DIKeyState(DIK_W) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_UP) & 0x80)
    {
        moveDir += g_WorldLook;
        ++axis_Z;
    }
    if (inputMgr->Get_DIKeyState(DIK_S) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_DOWN) & 0x80)
    {
        moveDir -= g_WorldLook;
        --axis_Z;
    }
    if (inputMgr->Get_DIKeyState(DIK_D) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_RIGHT) & 0x80)
    {
        moveDir += g_WorldRight;
        ++axis_X;
    }
    if (inputMgr->Get_DIKeyState(DIK_A) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_LEFT) & 0x80)
    {
        moveDir -= g_WorldRight;
        --axis_X;
    }

    // ---------------------------------
    // 2. 상태 및 속도 결정
    // ---------------------------------
    const bool isMoving = (D3DXVec3LengthSq(&moveDir) > 0.f);
    const auto speed    = m_OwnerCombatStat->Get_Speed();
    ACTORSTATE nextState;

    if (isMoving)
    {
        nextState = ACTORSTATE::RUN;

        // TODO 석호: 예시 코드. 상태 추가시 이런 식으로 변경
        //if (GetAsyncKeyState('Z') & 0x8000) 
        //{
        //    eNextState = ACTORSTATE::ATTACK;
        //    fCurrentSpeed = 5.f; // 공격 중 이동 속도 감소
        //}
    }
    else
    {
        //if (GetAsyncKeyState('Z') & 0x8000) 
        //{
        //    eNextState = ACTORSTATE::ATTACK;
        //}

        nextState = ACTORSTATE::IDLE;
    }

    // ---------------------------------
    // 3. 실제 적용
    // ---------------------------------
    m_OwnerStateCom->Change_State(nextState);

    if (isMoving)
    {
        D3DXVec3Normalize(&moveDir, &moveDir);
        m_TransformCom->Move_Pos(moveDir, timeDelta, speed);

        // 공격 중에는 방향 전환을 막고 싶다면 여기에 조건을 걸면 됨
        // if (eNextState != ACTORSTATE::ATTACK) 
        {
            ACTORDIR newDir = ACTORDIR::LEFT;

            if (axis_Z > 0) // 위
            {
                if (axis_X > 0)
                    newDir = ACTORDIR::R_UP;
                else if (axis_X < 0)
                    newDir = ACTORDIR::L_UP;
                else
                    newDir = ACTORDIR::UP;
            }
            else if (axis_Z < 0) // 아래
            {
                if (axis_X > 0)
                    newDir = ACTORDIR::R_DOWN;
                else if (axis_X < 0)
                    newDir = ACTORDIR::L_DOWN;
                else
                    newDir = ACTORDIR::DOWN;
            }
            else // Z축 입력 없음 (좌/우 만 입력)
            {
                if (axis_X > 0)
                    newDir = ACTORDIR::RIGHT;
                else if (axis_X < 0)
                    newDir = ACTORDIR::LEFT;
            }

            m_OwnerStateCom->Change_Dir(newDir);
        }
    }
}

CPlayerController* CPlayerController::Create(DEVICE graphicDev)
{
    auto controller = new CPlayerController(graphicDev);

    if (FAILED(controller->Ready_PlayerController()))
    {
        Safe_Release(controller);
        MSG_BOX("PlayerController Create Failed");
        return nullptr;
    }

    return controller;
}

CComponent* CPlayerController::Clone()
{
    return new CPlayerController(*this);
}

void CPlayerController::Free()
{
    m_OwnerStateCom   = nullptr;
    m_OwnerCombatStat = nullptr;

    CComponent::Free();
}
