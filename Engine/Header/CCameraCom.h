#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CTransform;

// TODO 석호: 카메라 오브젝트, 다중 카메라 관리용 카메라 매니저 구성할 것

/// @brief 컴포넌트 기반의 View/Proj 변환용 컴포넌트
class ENGINE_DLL CCameraCom : public CComponent
{
public:
    // 시점 모드
    enum VIEW_MODE
    {
        VIEW_FPS,               // 1인칭
        VIEW_TPS,               // 3인칭
        VIEW_QUARTER,           // 쿼터뷰 (2.5D)
        VIEW_END
    };

    // 투영 타입
    enum PROJ_TYPE
    {
        PROJ_PERSPECTIVE,       // 원근 투영
        PROJ_ORTHOGRAPHIC,      // 직교 투영
        PROJ_END
    };

    // TODO 석호: UI는 메인 카메라가 같이 띄우되, CRenderer::Render_UI 에서 직접 직교 투영을 쓰고 돌려놓는 식으로 하자
    // 그니까 이건 직교 투영을 월드에서 직접 쓰고 싶을때만 조작하게 되는거

private:
    explicit CCameraCom();
    explicit CCameraCom(DEVICE graphicDev);
    explicit CCameraCom(const CCameraCom& rhs);
    ~CCameraCom() override;

public:
    HRESULT Ready_Camera();
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    // 시점 모드 변경
    void Set_CamMode(VIEW_MODE viewMode)
    {
        if (m_ViewMode != viewMode && viewMode < VIEW_END)
            m_ViewMode = viewMode;

        // todo 석호: Owner는 카메라 오브젝트가 될 것이므로, 오브젝트의 시점 모드 역시 여기서 바꾸는 것을 시도해보자
    }

    // 투영 타입 변경
    void Set_ProjType(PROJ_TYPE projType)
    {
        if (m_ProjType != projType && projType < PROJ_END)
        {
            m_ProjType = projType;
            m_isDirty  = true;
        }
    }

    // TPS / QUARTER 카메라 오프셋 (타겟과의 거리) 설정
    void Set_Offset(const _vec3& offset) { m_Offset = offset; }

    void Set_FOV(const _float fov)
    {
        m_FOV     = fov;
        m_isDirty = true;
    }

    void Set_Aspect(const _float _aspect)
    {
        m_Aspect  = _aspect;
        m_isDirty = true;
    }

    void Set_Near(const _float _near)
    {
        m_Near    = _near;
        m_isDirty = true;
    }

    void Set_Far(const _float _far)
    {
        m_Far     = _far;
        m_isDirty = true;
    }

    VIEW_MODE Get_ViewMode() const { return m_ViewMode; }
    PROJ_TYPE Get_ProjType() const { return m_ProjType; }

    const D3DXMATRIX& Get_View() const { return m_matView; }
    const D3DXMATRIX& Get_Proj() const { return m_matProj; }

    const _vec3& Get_Offset() const { return m_Offset; }

    _float Get_FOV() const { return m_FOV; }
    _float Get_Aspect() const { return m_Aspect; }
    _float Get_Near() const { return m_Near; }
    _float Get_Far() const { return m_Far; }

private:
    void Compute_View_FPS();
    void Compute_View_TPS();
    void Compute_View_QUARTER();

    void Compute_Proj_Perspective();
    void Compute_Proj_Orthographic();

private:
    void Key_Input();

private:
    VIEW_MODE m_ViewMode;
    PROJ_TYPE m_ProjType;

    _matrix m_matView, m_matProj;

    _vec3 m_Offset;         // 3인칭 카메라의 타겟 기준 오프셋 (거리)

    _vec3 m_Pos, m_Look, m_Up, m_Eye, m_At;

    _float m_FOV, m_Aspect, m_Near, m_Far;

    _bool m_isDirty;        // 투영 변환 재연산 트리거

public:
    static CCameraCom* Create(DEVICE pGraphicDev);
    CComponent*        Clone() override;

private:
    void Free() override;
};

END
