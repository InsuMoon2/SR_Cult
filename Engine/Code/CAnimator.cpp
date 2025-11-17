#include "CAnimator.h"

#include "CGameObject.h"
#include "CRcTex.h"
#include "CSpriteAnimation.h"

CAnimator::CAnimator(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_CurAnimation(nullptr), m_Texture(nullptr), m_Play(false), m_BufferCom(nullptr)
{ }

CAnimator::CAnimator(const CAnimator& rhs)
    : CComponent(rhs),
      m_CurAnimation(nullptr), m_Texture(nullptr), m_Play(false), m_BufferCom(nullptr)
{ }

CAnimator::~CAnimator()
{ }

HRESULT CAnimator::Ready_Animator()
{
    if (m_Owner == nullptr)
        return E_FAIL;

    // TODO : Stage-> Create -> ReadyAnimator 호출 시키에는 OwnerSetting이 안돼있는데 어떻게 바꿀지?
    m_BufferCom = dynamic_cast<CRcTex*>(m_Owner->Get_Component(ID_STATIC, COMPONENTTYPE::RC_TEX));
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

    return S_OK;
}

_int CAnimator::Update_Component(const _float& timeDelta)
{
    if (m_Play == false || m_CurAnimation == nullptr || m_BufferCom == nullptr)
        return 0;

    _int exit = CComponent::Update_Component(timeDelta);

    // 1. 애니메이션 업데이트
    m_CurAnimation->Update(timeDelta);

    // 2. 현재 프레임에 해당하는 UV 계산
    _float u0, v0, u1, v1;
    m_CurAnimation->Get_UV(u0, v0, u1, v1);

    // 3. RcTex에 UV 반영하기
    m_BufferCom->Set_UV(u0, v0, u1, v1);

    return exit;
}

void CAnimator::LateUpdate_Component()
{
    CComponent::LateUpdate_Component();
}

void CAnimator::Render_Component()
{
    // Render는 Renderer에서 진행
}

HRESULT CAnimator::Add_Animation(const wstring& key, CSpriteAnimation* animation)
{
    if (animation == nullptr)
        return E_FAIL;

    auto iter = m_Animations.find(key);

    if (iter != m_Animations.end())
    {
        MSG_BOX("Exits animation");
        return E_FAIL;
    }

    m_Animations.insert({ key, animation });

    return S_OK;
}

CSpriteAnimation* CAnimator::GetOrAdd_Animation(const wstring& key, CSpriteAnimation* animation)
{
    auto iter = m_Animations.find(key);

    if (iter != m_Animations.end())
        return iter->second;

    if (animation == nullptr)
        return nullptr;

    m_Animations.insert({ key, animation });

    return animation;
}

HRESULT CAnimator::Create_Animation(const wstring& key,
                                    _uint          maxX,
                                    _uint          maxY,
                                    _int           lineY,
                                    _float         interval,
                                    ANIMSTATE      state)
{
    CSpriteAnimation* anim = CSpriteAnimation::Create();

    anim->Init(maxX,
               maxY,
               0,
               lineY,
               interval,
               true,
               state);

    return Add_Animation(key, anim);
}

void CAnimator::Play_Animation(const wstring& Key)
{
    CSpriteAnimation* anim = Find_Animation(Key);

    if (anim == nullptr)
        return;

    anim->Reset(); // 초기화 이후 재생

    m_CurAnimation = anim;
    m_Play         = true;
}

void CAnimator::Stop_Animation()
{
    m_Play = false;
}

CAnimator* CAnimator::Create(DEVICE graphicDev)
{
    auto animator = new CAnimator(graphicDev);

    /*if (FAILED(animator->Ready_Animator()))
    {
        Safe_Release(animator);
        MSG_BOX("Animator Create Failed");
        return nullptr;
    }*/

    return animator;
}

CComponent* CAnimator::Clone()
{
    return new CAnimator(*this);
}

CSpriteAnimation* CAnimator::Find_Animation(const wstring& Key)
{
    auto iter = m_Animations.find(Key);

    if (iter == m_Animations.end())
        return nullptr;

    return iter->second;
}

void CAnimator::Free()
{
    CComponent::Free();

    for_each(m_Animations.begin(), m_Animations.end(), CDeleteMap());
    m_Animations.clear();
}
