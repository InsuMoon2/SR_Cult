#pragma once
#include "CComponent.h"

BEGIN(Engine)
class CRcTex;
class CSpriteAnimation;
class CTexture;

class ENGINE_DLL CAnimator : public CComponent
{
private:
    explicit CAnimator(DEVICE graphicDev);
    explicit CAnimator(const CAnimator& rhs);
    ~CAnimator() override;

public:
    HRESULT Ready_Animator();

    _int Update_Component(const _float& timeDelta) override;
    void LateUpdate_Component() override;
    void Render_Component();

public:
    // 애니메이션 등록 (이미 만들어진 CSpriteAnimation 넣기)
    HRESULT           Add_Animation(const wstring& key, CSpriteAnimation* animation);
    CSpriteAnimation* GetOrAdd_Animation(const wstring& key, CSpriteAnimation* animation);

    HRESULT Create_Animation(const wstring& key,
                             _uint          maxX,      // 가로 최대 프레임
                             _uint          maxY,      // 세로 최대 프레임
                             _int           lineY,     // 몇 번째 줄에 있는지
                             _float         interval, // 다음 프레임으로 넘어가는 시간
                             ANIMSTATE      state = ANIMSTATE::LOOP); // Loop ? Stop ?

    void Play_Animation(const wstring& Key);
    void Stop_Animation();

    CSpriteAnimation* Get_CurAnimation() const { return m_CurAnimation; }

    static CAnimator* Create(DEVICE graphicDev);
    CComponent*       Clone() override;

private:
    CSpriteAnimation* Find_Animation(const wstring& Key);

    void Free() override;

private:
    map<wstring, CSpriteAnimation*> m_Animations;
    CSpriteAnimation*               m_CurAnimation;
    CTexture*                       m_Texture;
    _bool                            m_Play;

    Engine::CRcTex* m_BufferCom;
};

END
