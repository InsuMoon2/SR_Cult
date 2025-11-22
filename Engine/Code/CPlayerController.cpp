#include "CPlayerController.h"

#include "CDInputMgr.h"
#include "CGameObject.h"
#include "CState.h"
#include "CTransform.h"

CPlayerController::CPlayerController(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_OwnerStateCom(nullptr)
{ }

CPlayerController::CPlayerController(const CPlayerController& rhs)
    : CComponent(rhs),
      m_OwnerStateCom(nullptr)
{ }

CPlayerController::~CPlayerController()
{ }

HRESULT CPlayerController::Ready_PlayerController()
{
    return S_OK;
}

_int CPlayerController::Update_Component(const _float& timeDelta)
{
    if (nullptr == m_OwnerStateCom)
        Init_OwnerComponent();
    
    Key_Input(timeDelta);

    return 0;
}

void CPlayerController::LateUpdate_Component()
{ }

void CPlayerController::Init_OwnerComponent()
{
    m_OwnerStateCom =
        dynamic_cast<CState*>(m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::STATE));

    NULL_CHECK_MSG(m_OwnerStateCom, L"CPlayerController : Owner's State init setting failed");;
}

void CPlayerController::Key_Input(const _float& timeDelta)
{
    auto inputMgr = CDInputMgr::GetInstance();

    // ---------------------------------
    // 1. 방향 입력 벡터 계산
    // ---------------------------------
    _vec3 look  = g_WorldLook;
    _vec3 right = g_WorldRight;

    _vec3 moveDir = { 0.f, 0.f, 0.f };
    int   axis_X{};
    int   axis_Z{};

    if (inputMgr->Get_DIKeyState(DIK_W) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_UP) & 0x80)
    {
        moveDir += look;
        ++axis_Z;
    }
    if (inputMgr->Get_DIKeyState(DIK_S) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_DOWN) & 0x80)
    {
        moveDir -= look;
        --axis_Z;
    }
    if (inputMgr->Get_DIKeyState(DIK_D) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_RIGHT) & 0x80)
    {
        moveDir += right;
        ++axis_X;
    }
    if (inputMgr->Get_DIKeyState(DIK_A) & 0x80 ||
        inputMgr->Get_DIKeyState(DIK_LEFT) & 0x80)
    {
        moveDir -= right;
        --axis_X;
    }

    // ---------------------------------
    // 2. 상태 및 속도 결정
    // ---------------------------------
    bool isMoving = (D3DXVec3LengthSq(&moveDir) > 0.f);

    ACTORSTATE nextState;
    _float     curSpeed{};

    if (isMoving)
    {
        nextState = ACTORSTATE::RUN;
        curSpeed  = 10.f;

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
        m_TransformCom->Move_Pos(moveDir, timeDelta, curSpeed);

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
    CComponent::Free();
}
