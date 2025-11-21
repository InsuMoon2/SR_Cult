#pragma once

#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CAnimator;

/**
 * @brief GameObject의 전투 상태 스테이터스 컴포넌트
 */
class ENGINE_DLL CCombatStat : public CComponent
{
private:
    explicit CCombatStat(DEVICE GraphicDev);
    explicit CCombatStat(const CCombatStat& rhs);
    ~CCombatStat() override;

public:
    HRESULT Ready_StateComponent(DEVICE GraphicDev);
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    _float Get_Attack() { return m_Attack; }
    _float Get_Hp() { return m_Hp; }
    _float Get_MaxHp() { return m_maxHp; }
    _float Get_Mp() { return m_Mp; }
    _float Get_MaxMp() { return m_maxMp; }

    void Set_Attack(float attack) { m_Attack = attack; }
    void Set_Hp(float hp) { m_Hp = hp; }
    void Set_MaxHp(float maxHp) { m_maxHp = maxHp; }
    void Set_Mp(float mp) { m_Mp = mp; }

public:
    static CCombatStat* Create(DEVICE GraphicDev);

    CComponent* Clone() override;

protected:
    void Free() override;

private:
    _float m_Attack;

    _float m_Hp;
    _float m_maxHp =6.f;
    _float m_Mp;
    _float m_maxMp = 10.f;
};

END
