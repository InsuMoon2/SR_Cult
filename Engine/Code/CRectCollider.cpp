#include "CRectCollider.h"
#include "CSphereCollider.h"
#include "CRcCol.h"
#include "CTransform.h"

CRectCollider::CRectCollider(DEVICE GraphicDev)
    : CCollider(GraphicDev, COLLIDERTYPE::Rect)
    , m_BufferCom(nullptr), m_Size(1.f, 1.f)

{

}

CRectCollider::CRectCollider(const CRectCollider& rhs)
    : CCollider(rhs) , m_BufferCom(rhs.m_BufferCom), m_Size(rhs.m_Size)
{
    if (m_BufferCom)
        m_BufferCom->AddRef();
}

CRectCollider::~CRectCollider()
{

}

HRESULT CRectCollider::Ready_BoxCollider(DEVICE GraphicDev)
{
    if (FAILED(CCollider::Ready_Collider()))   
        return E_FAIL;

    // 렌더용 RcCol Create, 일단 테스트 후 프로토타입으로 변경
    m_BufferCom = CRcCol::Create(GraphicDev);

    if (m_BufferCom == nullptr)
        return E_FAIL;

    return S_OK;
}

_int CRectCollider::Update_Component(const _float& fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);



    return iExit;
}

void CRectCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();


}

void CRectCollider::Render()
{
    CCollider::Render();

    if (m_BufferCom == nullptr)
        return;

    CTransform* transform = Get_Transform();
    if (transform == nullptr)
        return;

    // 월드 행렬 가져와서,
    _matrix matWorld = *transform->Get_World();

    // RectCollider Scale 반영
    _matrix matScale;
    D3DXMatrixScaling(&matScale, m_Size.x * 0.5f, m_Size.y * 0.5f, 1.f);

    matWorld = matScale * matWorld;

    m_GraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    // Collider는 와이어프레임으로 보이기
    DWORD prevFillMode = 0; // 현재 FillMode 저장
    m_GraphicDev->GetRenderState(D3DRS_FILLMODE, &prevFillMode);
    m_GraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    // Rect 그리기
    m_BufferCom->Render_Buffer();

    m_GraphicDev->SetRenderState(D3DRS_FILLMODE, prevFillMode); // 복원
}

bool CRectCollider::CheckCollision(CCollider* other)
{
    switch (other->GetColliderType())
    {
    case COLLIDERTYPE::Rect:
        return CheckCollisionBox2Box(this, static_cast<CRectCollider*>(other));

    case COLLIDERTYPE::Sphere:
        return CheckCollisionSphere2Box(static_cast<CSphereCollider*>(other), this);

    }

    return false;
}

CRectCollider* CRectCollider::Create(DEVICE GraphicDev)
{
    CRectCollider* box = new CRectCollider(GraphicDev);

    if (FAILED(box->Ready_BoxCollider(GraphicDev)))
    {
        Safe_Release(box);
        MSG_BOX("BoxCollider Create Failed");
        return nullptr;
    }

    return box;
}

CComponent* CRectCollider::Clone()
{
    CRectCollider* col = new CRectCollider(*this);

    col->Ready_Collider();     

    return col;
}

void CRectCollider::Free()
{
    CCollider::Free();

    Safe_Release(m_BufferCom);
}
