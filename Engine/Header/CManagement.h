#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CComponent;
class CGameObject;
class CScene;

/// @brief Scene의 생성/교체/업데이트를 총괄하는 엔진의 최상위 관리자
/// @details
/// - Scene 전환 시 메모리를 안전하게 해제하고, 외부 시스템의 접근 지점을 단일화한다.
/// - 게임 루프 단계(Update/LateUpdate/Render)를 Scene에 위임해 실행 순서를 제어한다.
class ENGINE_DLL CManagement : public CBase
{
    DECLARE_SINGLETON(CManagement)

private:
    explicit CManagement();
    ~CManagement() override;

public:
    CComponent* Get_Component(COMPONENTID   componentID,
                              LAYERTYPE     layerType,
                              OBJTYPE       objType,
                              COMPONENTTYPE componentType);

    // CManagement 가 자체적으로 보유중인 플레이어 객체 유무
    CGameObject* Get_PlayerObject() const { return m_PlayerObject; }
    // CManagement 가 자체적으로 보유중인 카메라 객체 유무
    CGameObject* Get_CameraObject() const { return m_CameraObject; }

public:
    HRESULT Cache_PersistentObject(OBJTYPE objType, CGameObject* gameObject);

public:
    HRESULT Set_Scene(CScene* scene);   // 강제 씬 전환. 게임 시작시에만 사용중
    _int    Update_Scene(const _float& timeDelta);
    void    LateUpdate_Scene(const _float& timeDelta);
    void    Render_Scene(LPDIRECT3DDEVICE9 graphicDev);

public:
    HRESULT Request_ChangeScene(CScene* nextScene);    // 씬 전환 예약
    void    Commit_ChangeScene();   // 프레임 경계(다음 프레임 업데이트 시작)에서 씬 실제 전환

private:
    CScene* m_Scene;                // 현재 씬
    CScene* m_NextScene;            // 전환 예약 씬

    _bool m_ShouldChangeScene;      // 전환 플래그

    CGameObject* m_PlayerObject;    // 씬 전환 시 유지할 플레이어
    CGameObject* m_CameraObject;    // 씬 전환 시 유지할 메인 카메라

public:
    void Free() override;
};

END
