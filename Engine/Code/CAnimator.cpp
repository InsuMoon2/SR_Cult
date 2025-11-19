#include "CAnimator.h"

#include "CGameObject.h"
#include "CRcTex.h"
#include "CSpriteAnimation.h"
#include "CTexture.h"

CAnimator::CAnimator(DEVICE graphicDev)
    : CComponent(graphicDev),
      m_CurAnimation(nullptr), m_Play(false), m_BufferCom(nullptr), m_Texture(nullptr), m_TextureType(COMPONENTTYPE::COMPONENT_END)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
    : CComponent(rhs),
      m_CurAnimation(nullptr), m_Play(false), m_BufferCom(nullptr), m_Texture(nullptr), m_TextureType(rhs.m_TextureType)
{
}

CAnimator::~CAnimator()
{ }

HRESULT CAnimator::Ready_Animator()
{
    if (m_Owner == nullptr)
        return E_FAIL;

    m_BufferCom = dynamic_cast<CRcTex*>(m_Owner->Get_Component(ID_STATIC, COMPONENTTYPE::RC_TEX));
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

    // 주의! Object마다 Set_TextureType 설정 꼭 해줘야함
    // ex) Player -> m_AnimatorCom->Set_TextureType(COMPONENTTYPE::TEX_PLAYER);

    m_Texture = dynamic_cast<CTexture*>(m_Owner->Get_Component(ID_STATIC, m_TextureType));
    NULL_CHECK_RETURN(m_Texture, E_FAIL);

    return S_OK;
}

_int CAnimator::Update_Component(const _float& timeDelta)
{
    if (m_Play == false || m_CurAnimation == nullptr || m_Texture == nullptr)
        return 0;

    _int exit = CComponent::Update_Component(timeDelta);

    // 애니메이션 업데이트
    m_CurAnimation->Update(timeDelta);

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

HRESULT CAnimator::Create_Animation(const wstring& key, _uint frameCount, _float interval)
{
    CSpriteAnimation* anim
        = CSpriteAnimation::Create(frameCount, interval);

    NULL_CHECK_RETURN(anim, E_FAIL);

    return Add_Animation(key, anim);
}

void CAnimator::Play_Animation(const wstring& key, ANIMSTATE state, bool reset)
{
	CSpriteAnimation* anim = Find_Animation(key);
	if (anim == nullptr)
		return;

	// 플레이 중에 같은 애니메이션, 상태가 같으면 변경 X
	if (m_Play && m_CurAnimation == anim && anim->GetState() == state)
		return;

	m_CurAnimation = anim;
	m_CurKey = key;

	m_CurAnimation->SetState(state);

	if (reset)
		m_CurAnimation->Reset();

	// STOP이면 False, 나머지는 True값 세팅
	m_Play = (state != ANIMSTATE::STOP);
}

void CAnimator::Stop_Animation()
{
    m_Play = false;
}

_int CAnimator::Get_CurFrame()
{
    if (m_CurAnimation == nullptr)
        return 0;

    return m_CurAnimation->Get_Frame();
}

void CAnimator::Set_TextureType(COMPONENTTYPE type)
{
    m_TextureType = type;

    if (m_Owner && m_Initialized == false)
    {
        if (SUCCEEDED(Ready_Animator()))
            m_Initialized = true;

        else
            MSG_BOX("Animator Ready Failed");
    }
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
