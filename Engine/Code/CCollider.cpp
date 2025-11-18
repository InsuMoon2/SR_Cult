#include "CCollider.h"

#include "CBoxCollider.h"
#include "CCollisionManager.h"
#include "CRectCollider.h"
#include "CSphereCollider.h"
#include "CTransform.h"

CCollider::CCollider(DEVICE graphicDev, COLLIDERTYPE colliderType)
    : CComponent(graphicDev), m_ColliderType(colliderType)
{ }

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs), m_ColliderType(rhs.m_ColliderType)
{ }

CCollider::~CCollider()
{ }

HRESULT CCollider::Ready_Collider()
{
    CCollisionManager::GetInstance()->AddCollider(this);

    return S_OK;
}

_int CCollider::Update_Component(const _float& timeDelta)
{
    _int exit = CComponent::Update_Component(timeDelta);

    return exit;
}

void CCollider::LateUpdate_Component()
{
    CComponent::LateUpdate_Component();
}

void CCollider::Render()
{
    // 디버깅용 Render가 필요하긴 할듯
}

bool CCollider::CheckCollision(CCollider* other)
{
    // 파생 클래스에서 구현
    return false;
}

bool CCollider::IsOverlapping(CCollider* other)
{
    bool check = m_ColliderSet.find(other) != m_ColliderSet.end();

    return check;
}

void CCollider::AddOverlap(CCollider* other)
{
    m_ColliderSet.insert(other);
}

void CCollider::RemoveOverlap(CCollider* other)
{
    m_ColliderSet.erase(other);
}

// AABB(Box) vs AABB(Box)
bool CCollider::CheckCollisionRect2Rect(CRectCollider* b1, CRectCollider* b2)
{
    if (b1 == nullptr || b2 == nullptr)
        return false;

    CTransform* t1 = b1->Get_Transform();
    CTransform* t2 = b2->Get_Transform();

    if (t1 == nullptr || t2 == nullptr)
        return false;

    // 중심 좌표 (Transform의 INFO_POS 기준, XY 평면에서만 충돌 체크)
    _vec3 p1 = t1->Get_Pos();
    _vec3 p2 = t2->Get_Pos();

    _vec2 s1 = b1->Get_Size();
    _vec2 s2 = b2->Get_Size();

    _float halfW1 = s1.x * 0.5f;
    _float halfH1 = s1.y * 0.5f;
    _float halfW2 = s2.x * 0.5f;
    _float halfH2 = s2.y * 0.5f;

    _float minX1 = p1.x - halfW1;
    _float maxX1 = p1.x + halfW1;
    _float minY1 = p1.y - halfH1;
    _float maxY1 = p1.y + halfH1;

    _float minX2 = p2.x - halfW2;
    _float maxX2 = p2.x + halfW2;
    _float minY2 = p2.y - halfH2;
    _float maxY2 = p2.y + halfH2;

    // 한 쪽이 완전히 왼쪽/오른쪽/위/아래에 있으면 비충돌
    if (maxX2 < minX1) return false;
    if (maxX1 < minX2) return false;
    if (maxY2 < minY1) return false;
    if (maxY1 < minY2) return false;

    return true;
}

bool CCollider::CheckCollisionBox2Box(CBoxCollider* b1, CBoxCollider* b2)
{
    NULL_CHECK_RETURN(b1, false);
    NULL_CHECK_RETURN(b2, false);

    CTransform* t1 = b1->Get_Transform();
    CTransform* t2 = b2->Get_Transform();

    NULL_CHECK_RETURN(t1, false);
    NULL_CHECK_RETURN(t2, false);

    _vec3 p1 = t1->Get_Pos();
    _vec3 p2 = t2->Get_Pos();

    //_vec2 s1 = b1->Get_Size();
    //_vec2 s2 = b2->Get_Size();

}

// Circle(Sphere) vs AABB(Box)
bool CCollider::CheckCollisionSphere2Box(CSphereCollider* s1, CBoxCollider* b2)
{
    if (s1 == nullptr || b2 == nullptr)
        return false;

    CTransform* ts = s1->Get_Transform();
    CTransform* tb = b2->Get_Transform();
    if (ts == nullptr || tb == nullptr)
        return false;

    // 원 중심 (XY)
    _vec3 cs3, cb3;
    ts->Get_Info(INFO_POS, &cs3);
    tb->Get_Info(INFO_POS, &cb3);

    _float radius  = s1->Get_Radius();
    _vec2  boxSize;// = b2->Get_Size();

    _float halfW = boxSize.x * 0.5f;
    _float halfH = boxSize.y * 0.5f;

    _float minX = cb3.x - halfW;
    _float maxX = cb3.x + halfW;
    _float minY = cb3.y - halfH;
    _float maxY = cb3.y + halfH;

    // Box AABB 안에서 원 중심에 가장 가까운 점(clamp)
    _float closestX = cs3.x;

    if (closestX < minX)
        closestX = minX;

    else if (closestX > maxX)
        closestX = maxX;

    _float closestY = cs3.y;

    if (closestY < minY)
        closestY = minY;

    else if (closestY > maxY)
        closestY = maxY;

    _float dx     = cs3.x - closestX;
    _float dy     = cs3.y - closestY;
    _float distSq = dx * dx + dy * dy;

    return distSq <= (radius * radius);
}

// Circle vs Circle
bool CCollider::CheckCollisionSphere2Sphere(CSphereCollider* s1, CSphereCollider* s2)
{
    if (s1 == nullptr || s2 == nullptr)
        return false;

    CTransform* t1 = s1->Get_Transform();
    CTransform* t2 = s2->Get_Transform();
    if (t1 == nullptr || t2 == nullptr)
        return false;

    _vec3 p1, p2;
    t1->Get_Info(INFO_POS, &p1);
    t2->Get_Info(INFO_POS, &p2);

    _float r1 = s1->Get_Radius();
    _float r2 = s2->Get_Radius();

    _float dx = p1.x - p2.x;
    _float dy = p1.y - p2.y;

    _float distSq = dx * dx + dy * dy;
    _float sum    = r1 + r2;

    return distSq <= (sum * sum);
}
