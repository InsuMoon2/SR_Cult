#include "CSphereCollider.h"
#include "CRectCollider.h"

CSphereCollider::CSphereCollider(DEVICE GraphicDev)
    : CCollider(GraphicDev, COLLIDERTYPE::Sphere)
{

}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
    : CCollider(rhs), m_Radius(rhs.m_Radius)
{
}

CSphereCollider::~CSphereCollider()
{

}

HRESULT CSphereCollider::Ready_SphereCollider(DEVICE GraphicDev)
{

    return S_OK;
}

_int CSphereCollider::Update_Component(const _float& fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);



    return iExit;
}

void CSphereCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();


}

void CSphereCollider::Render()
{
    CCollider::Render();
}

bool CSphereCollider::CheckCollision(CCollider* other)
{
    switch (other->GetColliderType())
    {
    case COLLIDERTYPE::Rect:
        return CheckCollisionSphere2Box(this, static_cast<CRectCollider*>(other));

    case COLLIDERTYPE::Sphere:
        return CheckCollisionSphere2Sphere(this, static_cast<CSphereCollider*>(other));
    }

    return false;
}

CSphereCollider* CSphereCollider::Create(DEVICE GraphicDev)
{
    CSphereCollider* sphere = new CSphereCollider(GraphicDev);

    if (FAILED(sphere->Ready_SphereCollider(GraphicDev)))
    {
        Safe_Release(sphere);
        MSG_BOX("SphereCollider Create Failed");
        return nullptr;
    }

    return sphere;
}

CComponent* CSphereCollider::Clone()
{
    return new CSphereCollider(*this);
}

void CSphereCollider::Free()
{
    CCollider::Free();
}
