#pragma once
#include "CComponent.h"

//TODO 미완성. 추후 플레이어 조작을 이쪽으로 옮길 예정

BEGIN(Engine)
class CState;

/// @brief 플레이어의 조작을 담당하는 컴포넌트
class ENGINE_DLL CPlayerController : public CComponent
{
private:
    explicit CPlayerController(DEVICE graphicDev);
    explicit CPlayerController(const CPlayerController& rhs);
    ~CPlayerController() override;

public:
    HRESULT Ready_PlayerController();
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

private:
    void Init_OwnerComponent();     // m_OwnerStateCom 등 Owner의 컴포넌트를 가져와 사용하기 위한 초기 세팅
    void Key_Input(const _float& timeDelta);

private:
    CState* m_OwnerStateCom;        // 캐싱용

public:
    static CPlayerController* Create(DEVICE graphicDev);
    CComponent*               Clone() override;

protected:
    void Free() override;
};

END
