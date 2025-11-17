#include "CCameraCom.h"

#include "CDInputMgr.h"
#include "CTransform.h"

CCameraCom::CCameraCom()
    : m_ViewMode(VIEW_QUARTER), m_ProjType(PROJ_PERSPECTIVE),
      m_Offset(0.f, 4.f, -6.f),
      m_FOV(D3DXToRadian(60.f)), m_Aspect(static_cast<_float>(WINCX) / WINCY),
      m_Near(0.1f), m_Far(1000.f),
      m_isDirty(true)
{ }

CCameraCom::CCameraCom(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_ViewMode(VIEW_QUARTER), m_ProjType(PROJ_PERSPECTIVE),
      m_Offset(0.f, 4.f, -6.f),
      m_FOV(D3DXToRadian(60.f)), m_Aspect(static_cast<_float>(WINCX) / WINCY),
      m_Near(0.1f), m_Far(1000.f),
      m_isDirty(true)
{ }

CCameraCom::CCameraCom(const CCameraCom& rhs)
    : CComponent(rhs),
      m_ViewMode(rhs.m_ViewMode), m_ProjType(rhs.m_ProjType),
      m_Offset(rhs.m_Offset),
      m_FOV(rhs.m_FOV), m_Aspect(rhs.m_Aspect),
      m_Near(rhs.m_Near), m_Far(rhs.m_Far),
      m_isDirty(rhs.m_isDirty)
{ }

CCameraCom::~CCameraCom()
{ }

HRESULT CCameraCom::Ready_Camera()
{
    //m_Eye = { 0.f, 0.f, -10.f };   // 초기 카메라 위치
    //m_At  = { 0.f, 0.f, 10.f };    // 초기 카메라가 보는 지점
    //m_Up  = { 0.f, 1.f, 0.f };     // 월드 업 벡터

    m_Offset = { 0.f, 4.f, -6.f };

    //D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
    //D3DXMatrixPerspectiveFovLH(&m_matProj, m_FOV, m_Aspect, m_Near, m_Far);
    //
    //m_GraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    //m_GraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

    return S_OK;
}

_int CCameraCom::Update_Component(const _float& timeDelta)
{
    Key_Input();

    return 0;
}

void CCameraCom::LateUpdate_Component()
{
    if (m_TransformCom != nullptr)
    {
        m_Pos  = m_TransformCom->Get_Pos();
        m_Look = m_TransformCom->Get_Look();
        m_Up   = m_TransformCom->Get_Up();
    }

    switch (m_ViewMode)
    {
    case VIEW_FPS:
        Compute_View_FPS();
        break;
    case VIEW_TPS:
        Compute_View_TPS();
        break;
    case VIEW_QUARTER:
        Compute_View_QUARTER();
        break;
    default:
        break;
    }

    m_GraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

    // 투영 타입 변경 시에만 한 번 실행
    if (m_isDirty)
    {
        switch (m_ProjType)
        {
        case PROJ_PERSPECTIVE:
            Compute_Proj_Perspective();
            break;
        case PROJ_ORTHOGRAPHIC:
            Compute_Proj_Orthographic();
            break;
        default:
            break;
        }

        m_GraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
        m_isDirty = false;
    }
}

void CCameraCom::Compute_View_FPS()
{
    m_Eye = m_Pos;
    m_At  = m_Pos + m_Look;

    D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
}

void CCameraCom::Compute_View_TPS()
{
    _matrix matRotX, matRotY;
    D3DXMatrixRotationX(&matRotX, m_TransformCom->Get_Rotation(ROT_X));
    D3DXMatrixRotationY(&matRotY, m_TransformCom->Get_Rotation(ROT_Y));

    _matrix matRot = matRotY * matRotX;

    _vec3 vLocalOffset;
    D3DXVec3TransformCoord(&vLocalOffset, &m_Offset, &matRot);

    m_Eye = m_Pos + vLocalOffset;
    m_At  = m_Pos;

    const _vec3 vWorldUp = { 0, 1, 0 };

    D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &vWorldUp);
}

void CCameraCom::Compute_View_QUARTER()
{
    m_Eye = m_Pos + m_Offset;
    m_At  = m_Pos;

    const _vec3 vWorldUp = { 0, 1, 0 };

    D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &vWorldUp);
}

void CCameraCom::Compute_Proj_Perspective()
{
    D3DXMatrixPerspectiveFovLH(&m_matProj, m_FOV, m_Aspect, m_Near, m_Far);
}

void CCameraCom::Compute_Proj_Orthographic()
{
    D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
}

void CCameraCom::Key_Input()
{
#pragma region debug : 키 입력으로 카메라 시점 전환

    auto pInputMgr = CDInputMgr::GetInstance();

    if (pInputMgr->Get_DIKeyState(DIK_8))
        Set_CamMode(VIEW_FPS);
    else if (pInputMgr->Get_DIKeyState(DIK_9))
        Set_CamMode(VIEW_TPS);
    else if (pInputMgr->Get_DIKeyState(DIK_0))
        Set_CamMode(VIEW_QUARTER);

#pragma endregion
}

CCameraCom* CCameraCom::Create(DEVICE pGraphicDev)
{
    auto CameraCom = new CCameraCom(pGraphicDev);

    if (FAILED(CameraCom->Ready_Camera()))
    {
        Safe_Release(CameraCom);
        MSG_BOX("CCameraCom Create Failed");
        return nullptr;
    }

    return CameraCom;
}

CComponent* CCameraCom::Clone()
{
    return new CCameraCom(*this);
}

void CCameraCom::Free()
{
    //Safe_Release(m_TransformCom);
    // TODO 석호: 일단 AddRef를 안했으므로 지우진 말자

    CComponent::Free();
}
