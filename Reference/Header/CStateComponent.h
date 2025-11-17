#pragma once

#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CStateComponent : public CComponent
{
private:
    explicit CStateComponent(DEVICE GraphicDev);
    explicit CStateComponent(const CStateComponent& rhs);
    virtual  ~CStateComponent();

public:
    HRESULT Ready_StateComponent(DEVICE GraphicDev);
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;

public:
    void    Change_State(PLAYERSTATE State);
    void    Change_Dir(PLAYERDIR Dir);

    PLAYERSTATE Get_State();

    static CStateComponent* Create(DEVICE GraphicDev);
    CComponent* Clone() override;

protected:
    void Free() override;

private:
    PLAYERSTATE m_State;
    PLAYERDIR   m_Dir;
    

};

END
