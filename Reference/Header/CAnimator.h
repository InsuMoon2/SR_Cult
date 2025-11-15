#pragma once

#include "CComponent.h"

BEGIN(Engine)

class CRcTex;
class CSpriteAnimation;
class CTexture;

class ENGINE_DLL CAnimator : public CComponent
{
private:
	explicit CAnimator(DEVICE GraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual  ~CAnimator();

public:
	HRESULT Ready_Animator();

	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render_Component();

public:
    // 애니메이션 등록 (이미 만들어진 CSpriteAnimation 넣기)
    HRESULT Add_Animation(const wstring& Key, CSpriteAnimation* Animation);
    CSpriteAnimation*  GetOrAdd_Animation(const wstring& Key, CSpriteAnimation* Animation);

    HRESULT Create_Animation(const wstring& Key,
                            _uint maxX,      // 가로 최대 프레임
                            _uint maxY,      // 세로 최대 프레임
                            _int  lineY,     // 몇 번째 줄에 있는지
                            _float interval, // 다음 프레임으로 넘어가는 시간
                            ANIMSTATE state = ANIMSTATE::Loop); // Loop ? Stop ?

    void Play_Animation(const wstring& Key);
    void Stop_Animation();

    CSpriteAnimation* Get_CurAnimation() const { return m_CurAnimation; }

    static CAnimator* Create(DEVICE GraphicDev);
    virtual CComponent* Clone() override;

private:
    CSpriteAnimation* Find_Animation(const wstring& Key);

    virtual void Free() override;

private:
    map<wstring, CSpriteAnimation*> m_Animations;
    CSpriteAnimation*               m_CurAnimation;
    CTexture*                       m_Texture;
    bool                            m_Play;

    Engine::CRcTex*                 m_BufferCom;
};

END
