#include "pch.h"
#include "CMainCamera.h"

#include "CCameraCom.h"
#include "CTransform.h"

CMainCamera::CMainCamera(DEVICE graphicDev)
    : CGameObject(graphicDev),
      m_CameraCom(nullptr),
      m_TransformCom(nullptr),
      m_TargetTransformCom(nullptr),
      m_ViewMode(CCameraCom::VIEW_QUARTER)
{ }

CMainCamera::CMainCamera(const CMainCamera& rhs)
    : CGameObject(rhs),
      m_ViewMode(rhs.m_ViewMode)
{ }

CMainCamera::~CMainCamera()
{ }

HRESULT CMainCamera::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CMainCamera::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    // TODO : 매 프레임 작업

    return exit;
}

void CMainCamera::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CMainCamera::Render_GameObject()
{ }

HRESULT CMainCamera::Set_Target(CTransform* targetTransform)
{
    NULL_CHECK_RETURN(targetTransform, E_FAIL)

    m_TargetTransformCom = targetTransform;

    return S_OK;
}

HRESULT CMainCamera::Add_Component()
{
    // todo 추가할 것
    return S_OK;
}

CMainCamera* CMainCamera::Create(DEVICE graphicDev)
{
    // todo 추가할 것
    return nullptr;
}

void CMainCamera::Free()
{
    CGameObject::Free();
}
