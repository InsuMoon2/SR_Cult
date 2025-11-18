#include "pch.h"
#include "CMainCamera.h"

#include "CCameraCom.h"
#include "CCreateHelper.h"
#include "CTransform.h"

CMainCamera::CMainCamera(DEVICE graphicDev)
    : CGameObject(graphicDev),
      m_CameraCom(nullptr),
      m_TransformCom(nullptr),
      m_TargetTransformCom(nullptr)
{ }

CMainCamera::CMainCamera(const CMainCamera& rhs)
    : CGameObject(rhs)
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
    // TODO : 모드 변경

    switch (m_CameraCom->Get_CamMode())
    {
    case CCameraCom::CAM_FREE:
        Key_Input(timeDelta);
        break;
    case CCameraCom::CAM_TARGET:
        Chase_CamTarget(timeDelta);
        break;
    default:
        break;
    }

    return exit;
}

void CMainCamera::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CMainCamera::Render_GameObject()
{ }

HRESULT CMainCamera::Set_CamTarget(CTransform* targetTransform)
{
    NULL_CHECK_RETURN(targetTransform, E_FAIL)

    m_TargetTransformCom = targetTransform;

    return S_OK;
}

HRESULT CMainCamera::Add_Component()
{
    // transform
    m_TransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_TransformCom });

    // Camera Component
    m_CameraCom = CreateProtoComponent<CCameraCom>(this, COMPONENTTYPE::CAMERA);
    NULL_CHECK_RETURN(m_CameraCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::CAMERA, m_CameraCom });

    return S_OK;
}

void CMainCamera::Key_Input(const _float& timeDelta)
{


}

void CMainCamera::Chase_CamTarget(const _float& timeDelta)
{
    m_TransformCom->Set_Pos(m_TargetTransformCom->Get_Pos());
}

CMainCamera* CMainCamera::Create(DEVICE graphicDev)
{
    auto mainCamera = new CMainCamera(graphicDev);

    if (FAILED(mainCamera->Ready_GameObject()))
    {
        MSG_BOX("MainCamera Create Failed");
        Safe_Release(mainCamera);
        return nullptr;
    }

    return mainCamera;
}

void CMainCamera::Free()
{
    CGameObject::Free();
}
