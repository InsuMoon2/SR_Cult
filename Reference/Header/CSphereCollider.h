#pragma once

#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CCollider
{
private:
    explicit CSphereCollider(DEVICE GraphicDev);
    explicit CSphereCollider(const CSphereCollider& rhs);
    virtual  ~CSphereCollider() override;

public:
    HRESULT Ready_SphereCollider(DEVICE GraphicDev);
    _int Update_Component(const _float& fTimeDelta) override;
    void LateUpdate_Component() override;
    void Render() override;

public:
    bool CheckCollision(CCollider* other) override;

    _float Get_Radius() const { return m_Radius; }
    void Set_Radius(_float radius) { m_Radius = radius; }

    static CSphereCollider* Create(DEVICE GraphicDev);
    CComponent* Clone() override;

protected:
    void Free() override;

private:
    _float m_Radius = 0.f;


};

END
