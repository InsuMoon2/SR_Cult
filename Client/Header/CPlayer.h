#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
class CAnimator;
class CProtoMgr;
class CRectCollider;
class CState;
END

class CPlayer : public CGameObject
{
private:
    explicit CPlayer(DEVICE graphicDev);
    explicit CPlayer(const CPlayer& rhs);
    ~CPlayer() override;

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;


    void Render_Setting();
    void Render_Reset();

    void OnBeginOverlap(CCollider* self, CCollider* other) override;
    void OnEndOverlap(CCollider* self, CCollider* other) override;

private:
    HRESULT Add_Component();
    void    Animation_Setting();
    void    Key_Input(const _float& timeDelta);

private:
    Engine::CRcTex*        m_BufferCom;
    Engine::CTransform*    m_TransformCom;
    Engine::CTexture*      m_TextureCom;
    Engine::CAnimator*     m_AnimatorCom;
    Engine::CRectCollider* m_RectColCom;
    Engine::CState*        m_StateCom;

    map<PLAYERSTATE, wstring> m_StateAnim;

    void TempImGuiRender();

public:
    float m_Hp=100.f;

public:
    static CPlayer* Create(DEVICE graphicDev);

private:
    void Free() override;
};
