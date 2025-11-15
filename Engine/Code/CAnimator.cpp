#include "CAnimator.h"

#include "CGameObject.h"
#include "CSpriteAnimation.h"
#include "CRcTex.h"

CAnimator::CAnimator(DEVICE GraphicDev)
	: CComponent(GraphicDev),
	  m_CurAnimation(nullptr), m_Texture(nullptr), m_Play(false)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	: CComponent(rhs)

{
	m_Play = false;
	m_CurAnimation = nullptr;
	m_BufferCom = nullptr;
}

CAnimator::~CAnimator()
{

}

HRESULT CAnimator::Ready_Animator()
{
    if (m_Owner == nullptr)
        return E_FAIL;

    // TODO : Stage-> Create -> ReadyAnimator 호출 시키에는 OwnerSetting이 안돼있는데 어떻게 바꿀지?
    m_BufferCom = dynamic_cast<CRcTex*>(m_Owner->Get_Component(ID_STATIC, COMPONENTTYPE::RcTex));
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

	return S_OK;
}

_int CAnimator::Update_Component(const _float& fTimeDelta)
{
	if (m_Play == false || m_CurAnimation == nullptr || m_BufferCom == nullptr)
		return 0;

	_int Exit = CComponent::Update_Component(fTimeDelta);

	// 1. 애니메이션 업데이트
	m_CurAnimation->Update(fTimeDelta);

	// 2. 현재 프레임에 해당하는 UV 계산
	_float u0, v0, u1, v1;
	m_CurAnimation->Get_UV(u0, v0, u1, v1);

	// 3. RcTex에 UV 반영하기
	m_BufferCom->Set_UV(u0, v0, u1, v1);

	return Exit;
}

void CAnimator::LateUpdate_Component()
{
	CComponent::LateUpdate_Component();


}

void CAnimator::Render_Component()
{
	// Render는 Renderer에서 진행

}

HRESULT CAnimator::Add_Animation(const wstring& Key, CSpriteAnimation* Animation)
{
	if (Animation == nullptr)
		return E_FAIL;

	auto iter = m_Animations.find(Key);

	if (iter != m_Animations.end())
	{
        MSG_BOX("Exits Animation");
		return E_FAIL;
	}

	m_Animations.insert({ Key, Animation });

	return S_OK;
}

CSpriteAnimation* CAnimator::GetOrAdd_Animation(const wstring& Key, CSpriteAnimation* Animation)
{
    auto iter = m_Animations.find(Key);

    if (iter != m_Animations.end())
        return iter->second;

    if (Animation == nullptr)
        return nullptr;

    m_Animations.insert({ Key, Animation });

    return Animation;
}

HRESULT CAnimator::Create_Animation(const wstring& Key, _uint maxX, _uint maxY, _int lineY, _float interval,
                                    ANIMSTATE state)
{
	CSpriteAnimation* anim = CSpriteAnimation::Create();

	anim->Init(maxX, maxY,
		0, lineY,
		interval,
		true,    
		state);

	return Add_Animation(Key, anim);
}

void CAnimator::Play_Animation(const wstring& Key)
{
	CSpriteAnimation* anim = Find_Animation(Key);

	if (anim == nullptr)
		return;

	anim->Reset(); // 초기화 이후 재생

	m_CurAnimation = anim;
	m_Play = true;
}

void CAnimator::Stop_Animation()
{
	m_Play = false;
}

CAnimator* CAnimator::Create(DEVICE GraphicDev)
{
	CAnimator* animator = new CAnimator(GraphicDev);

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
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator::Free()
{
	CComponent::Free();

	for_each(m_Animations.begin(), m_Animations.end(), CDeleteMap());
	m_Animations.clear();
}


