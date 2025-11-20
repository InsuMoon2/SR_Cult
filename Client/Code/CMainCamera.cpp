#include "pch.h"
#include "CMainCamera.h"

#include "CCameraCom.h"
#include "CCreateHelper.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CTransform.h"

CMainCamera::CMainCamera(DEVICE graphicDev)
    : CGameObject(graphicDev),
      m_CameraCom(nullptr),
      m_TransformCom(nullptr),
      m_TargetTransformCom(nullptr)
{ }

CMainCamera::CMainCamera(const CMainCamera& rhs)
    : CGameObject(rhs),
      m_CameraCom(nullptr),
      m_TransformCom(nullptr),
      m_TargetTransformCom(nullptr)
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

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

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
{
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    Render_ImGui();
}

HRESULT CMainCamera::Set_CamTarget(CTransform* targetTransform)
{
    // 같은 대상을 다시 설정하는 경우 무시
    if (m_TargetTransformCom == targetTransform)
        return S_OK;

    // 기존 타겟이 있다면 해제 (RefCnt 감소)
    Safe_Release(m_TargetTransformCom);

    // 새 타겟 설정
    m_TargetTransformCom = targetTransform;

    if (nullptr == m_TargetTransformCom)
    {
        m_CameraCom->Set_CamMode(CCameraCom::CAM_FREE);
    }
    else
    {
        // 새 타겟 참조 (RefCnt 증가)
        // 다른 오브젝트의 컴포넌트를 가져와서 오랫동안 참조해야 한다면
        // AddRef()를 통해 수명을 연장시켜야 안전합니다.
        m_TargetTransformCom->AddRef();
        m_CameraCom->Set_CamMode(CCameraCom::CAM_TARGET);
    }

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

    if (inputMgr->Get_DIKeyState(DIK_LSHIFT) & 0x80)
    {
        // Y축 회전
        if (inputMgr->Get_DIKeyState(DIK_J) & 0x80)
        {
            m_TransformCom->Rotation(ROT_Y, D3DXToRadian(-180), timeDelta);
            moving = true;
        }

        if (inputMgr->Get_DIKeyState(DIK_L) & 0x80)
        {
            m_TransformCom->Rotation(ROT_Y, D3DXToRadian(180), timeDelta);
            moving = true;
        }

        // X축 회전
        if (inputMgr->Get_DIKeyState(DIK_I) & 0x80)
        {
            m_TransformCom->Rotation_World(ROT_X, D3DXToRadian(-180), timeDelta);
            moving = true;
        }

        if (inputMgr->Get_DIKeyState(DIK_K) & 0x80)
        {
            m_TransformCom->Rotation_World(ROT_X, D3DXToRadian(180), timeDelta);
            moving = true;
        }
    }
    else
    {
        // 앞뒤
        dir = m_TransformCom->Get_Look();
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
        dir = m_TransformCom->Get_Right();
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
    }

    // 상하
    dir = m_TransformCom->Get_Up();
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
    if (m_CameraCom == nullptr)
        return;

    if (ImGui::Begin("MainCam Inspector"))
    {
        ImGui::Text(u8"카메라 Free 모드 조작 :");
        ImGui::TextWrapped(u8"I 앞 | K 뒤 | J 좌 | L 우 | P 상 | ; 하 | LShift+I 회전 상 | LShift+K 회전 하 | LShift+J 회전 좌 | LShift+L 회전 우");

        // ------------------------
        // CAM_MODE 편집
        // ------------------------
        const char* camModeItems[] = { "Free", "Target" };
        int         camMode        = static_cast<int>(m_CameraCom->Get_CamMode());
        if (ImGui::Combo("Camera Mode", &camMode, camModeItems, IM_ARRAYSIZE(camModeItems)))
            m_CameraCom->Set_CamMode(static_cast<CCameraCom::CAM_MODE>(camMode));

        // ------------------------
        // VIEW_TYPE 편집
        // ------------------------
        const char* viewTypeItems[] = { "FPS", "TPS", "Quarter" };
        int         viewType        = static_cast<int>(m_CameraCom->Get_ViewType());
        if (ImGui::Combo("View Type", &viewType, viewTypeItems, IM_ARRAYSIZE(viewTypeItems)))
            m_CameraCom->Set_ViewType(static_cast<CCameraCom::VIEW_TYPE>(viewType));

        // ------------------------
        // PROJ_TYPE 편집
        // ------------------------
        const char* projTypeItems[] = { "Perspective", "Orthographic" };
        int         projType        = static_cast<int>(m_CameraCom->Get_ProjType());
        if (ImGui::Combo("Projection Type", &projType, projTypeItems, IM_ARRAYSIZE(projTypeItems)))
            m_CameraCom->Set_ProjType(static_cast<CCameraCom::PROJ_TYPE>(projType));
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
    // 가지고 있던 타겟의 참조를 해제해야 메모리 누수가 발생하지 않습니다.
    Safe_Release(m_TargetTransformCom);

    CGameObject::Free();
}
