#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CRcTex;
class CBoxTex;
class CSpriteAnimation;
class CTexture;
class CVIBuffer;

class ENGINE_DLL CAnimator : public CComponent
{
private:
    explicit CAnimator(DEVICE graphicDev);
    explicit CAnimator(const CAnimator& rhs);
    ~CAnimator() override;

public:
    _int Update_Component(const _float& timeDelta) override;
    void LateUpdate_Component() override;
    void Render_Component();

public:
    // 애니메이션 등록 (이미 만들어진 CSpriteAnimation 넣기)
    HRESULT           Add_Animation(const wstring& key, CSpriteAnimation* animation);
    CSpriteAnimation* GetOrAdd_Animation(const wstring& key, CSpriteAnimation* animation);

    HRESULT Create_Animation(const wstring& key,
                             _uint          frameCount,
                             _float         interval);

    void Play_Animation(const wstring& key, ANIMSTATE state, bool reset = true);
    void Stop_Animation();

    CSpriteAnimation* Get_CurAnimation() const { return m_CurAnimation; }

    // 현재 재생중인 키값
    const wstring& Get_CurKey() { return m_CurKey; }
    // 현재 재생중인 프레임
    _int Get_CurFrame();

    // 주의! Object마다 Set_TextureType 설정 꼭 해줘야함
    void Set_TextureType(COMPONENTTYPE type);

    static CAnimator* Create(DEVICE graphicDev);
    CComponent*       Clone() override;

private:
    HRESULT           Ready_Animator();
    CSpriteAnimation* Find_Animation(const wstring& Key);

    void Free() override;

private:
    map<wstring, CSpriteAnimation*> m_Animations;
    CSpriteAnimation*               m_CurAnimation;
    wstring                         m_CurKey;

    _bool m_Play;

    //Engine::CRcTex*   m_BufferCom;
    Engine::CVIBuffer*  m_BufferCom;
    Engine::CTexture*   m_Texture;

    bool m_Initialized = false;

    COMPONENTTYPE m_TextureType;
};

END
