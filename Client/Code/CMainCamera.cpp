#include "pch.h"
#include "CMainCamera.h"

#include "CCameraCom.h"
#include "CCreateHelper.h"
#include "CDInputMgr.h"
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
{}

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
    auto inputMgr = CDInputMgr::GetInstance();

    const _float speed = 10.f;

    _vec3 dir    = { 0.f, 0.f, 0.f };
    bool  moving = false;

#pragma region 키 입력으로 카메라 시점 전환
    //TODO : 자유 모드에서만 작동중

    if (inputMgr->Get_DIKeyState(DIK_8) & 0x80)
        m_CameraCom->Set_ViewType(CCameraCom::VIEW_FPS);

    if (inputMgr->Get_DIKeyState(DIK_9) & 0x80)
        m_CameraCom->Set_ViewType(CCameraCom::VIEW_TPS);

    if (inputMgr->Get_DIKeyState(DIK_0) & 0x80)
        m_CameraCom->Set_ViewType(CCameraCom::VIEW_QUARTER);

#pragma endregion

    // 앞뒤
    m_TransformCom->Get_Info(INFO_LOOK, &dir);
    if (inputMgr->Get_DIKeyState(DIK_I) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, speed);
        moving = true;
    }

    if (inputMgr->Get_DIKeyState(DIK_K) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, -speed);
        moving = true;
    }

    // 좌우
    m_TransformCom->Get_Info(INFO_RIGHT, &dir);
    if (inputMgr->Get_DIKeyState(DIK_L) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, speed);
        moving = true;
    }

    if (inputMgr->Get_DIKeyState(DIK_J) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, -speed);
        moving = true;
    }

    // 상하
    m_TransformCom->Get_Info(INFO_UP, &dir);
    if (inputMgr->Get_DIKeyState(DIK_P) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, speed);
        moving = true;
    }

    if (inputMgr->Get_DIKeyState(DIK_SEMICOLON) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, -speed);
        moving = true;
    }
}

void CMainCamera::Chase_CamTarget(const _float& timeDelta)
{
    m_TransformCom->Set_Pos(m_TargetTransformCom->Get_Pos());
}

void CMainCamera::Render_ImGui()
{
    if (ImGui::Begin("MainCam Inspector"))
    {
        // TransformComponent
        if (m_TransformCom && ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const _vec3& pos = m_TransformCom->Get_Pos();

            ImGui::Text("Position");

            ImGui::Text("X :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosX", (float*)&pos.x);

            ImGui::Text("Y :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosY", (float*)&pos.y);

            ImGui::Text("Z :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosZ", (float*)&pos.z);

            m_TransformCom->Set_Pos(pos);
        }
    }

    ImGui::End();
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
