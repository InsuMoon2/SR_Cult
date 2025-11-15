#include "CComponent.h"
#include "CGameObject.h"
#include "CTransform.h"

CComponent::CComponent()
    : m_GraphicDev(nullptr), m_Clone(false), m_Owner(nullptr), m_TransformCom(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_GraphicDev(pGraphicDev), m_Clone(false), m_Owner(nullptr), m_TransformCom(nullptr)
{
    m_GraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
    : m_GraphicDev(rhs.m_GraphicDev), m_Clone(true), m_Owner(nullptr), m_TransformCom(nullptr)
{
    m_GraphicDev->AddRef();

}

CComponent::~CComponent()
{
}

void CComponent::Set_Owner(CGameObject* Owner)
{
    m_Owner = Owner;

    if (m_Owner)
    {
        m_TransformCom = dynamic_cast<CTransform*>(
            m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::Transform));
    }

}

void CComponent::Free()
{
    Safe_Release(m_GraphicDev);
}
