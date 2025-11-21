#include "CPlayerController.h"

CPlayerController::CPlayerController(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_pOwnerTransform(nullptr),
      m_pOwnerState(nullptr)
{
    // TODO 가져온 포인터 AddRef 할 것
}

CPlayerController::CPlayerController(const CPlayerController& rhs)
    : CComponent(rhs),
      m_pOwnerTransform(nullptr),
      m_pOwnerState(nullptr)
{ }

CPlayerController::~CPlayerController()
{ }

HRESULT CPlayerController::Ready_PlayerController()
{
    return S_OK;
}

_int CPlayerController::Update_Component(const _float& timeDelta)
{
    return 0;
}

void CPlayerController::LateUpdate_Component()
{}

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
