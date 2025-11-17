#pragma once
#include "CCameraCom.h"
#include "CGameObject.h"

// TODO : 미완성

BEGIN(Engine)
class CTransform;
END

class CMainCamera : public CGameObject
{
private:
    explicit CMainCamera(DEVICE graphicDev);
    explicit CMainCamera(const CMainCamera& rhs);
    ~CMainCamera() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

public:
    HRESULT Set_Target(CTransform* targetTransform);

public: // TODO : 카메라 이동, 카메라 쉐이킹 등 기능 함수

private:
    HRESULT Add_Component();

private:
    Engine::CCameraCom* m_CameraCom;
    Engine::CTransform* m_TransformCom;

    Engine::CTransform* m_TargetTransformCom;   // 따라가는 타겟의 Transform에 접근

    CCameraCom::VIEW_MODE m_ViewMode;

public:
    static CMainCamera* Create(DEVICE graphicDev);

private:
    void Free() override;
};
