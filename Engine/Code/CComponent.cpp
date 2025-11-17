#include "CComponent.h"
#include "CGameObject.h"
#include "CTransform.h"

CComponent::CComponent()
    : m_GraphicDev(nullptr), m_IsClone(false),
      m_Owner(nullptr), m_TransformCom(nullptr)
{ }

CComponent::CComponent(DEVICE graphicDev)
    : m_GraphicDev(graphicDev), m_IsClone(false),
      m_Owner(nullptr), m_TransformCom(nullptr)
{
    m_GraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
    : m_GraphicDev(rhs.m_GraphicDev), m_IsClone(true),
      m_Owner(nullptr), m_TransformCom(nullptr)
{
    m_GraphicDev->AddRef();
}

CComponent::~CComponent()
{ }

void CComponent::Set_Owner(CGameObject* owner)
{
    m_Owner = owner;

    if (m_Owner)
    {
        m_TransformCom = dynamic_cast<CTransform*>(
            m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM));
    }

}

void CComponent::Free()
{
    Safe_Release(m_GraphicDev);
}
