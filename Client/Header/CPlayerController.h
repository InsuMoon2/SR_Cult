#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CCombatStat;
class CState;
END

/// @brief 플레이어의 조작을 담당하는 컴포넌트
class CPlayerController : public CComponent
{
private:
    explicit CPlayerController(DEVICE graphicDev);
    explicit CPlayerController(const CPlayerController& rhs);
    ~CPlayerController() override;

public:
    HRESULT Ready_PlayerController();
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    // m_OwnerStateCom 등 Owner의 컴포넌트를 가져와 사용하기 위한 초기 세팅 함수
    HRESULT Init_OwnerComponent();

private:
    void Key_Input(const _float& timeDelta);

private:
#pragma region Caching owner's components

    Engine::CState*      m_OwnerStateCom;
    Engine::CCombatStat* m_OwnerCombatStat;

#pragma endregion

public:
    static CPlayerController* Create(DEVICE graphicDev);
    CComponent*               Clone() override;

protected:
    void Free() override;
};
