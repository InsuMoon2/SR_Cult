#include "pch.h"
#include "CPlayer.h"

#include "CAnimator.h"
#include "CCreateHelper.h"
#include "CDInputMgr.h"
#include "CEnumHelper.h"
#include "CRcTex.h"
#include "CRectCollider.h"
#include "CRenderer.h"
#include "CState.h"
#include "CTexture.h"
#include "CTransform.h"

CPlayer::CPlayer(DEVICE graphicDev)
    : CGameObject(graphicDev),
      m_BufferCom(nullptr),
      m_TransformCom(nullptr),
      m_TextureCom(nullptr),
      m_AnimatorCom(nullptr),
      m_RectColCom(nullptr),
      m_StateCom(nullptr)
{ }

CPlayer::CPlayer(const CPlayer& rhs)
    : CGameObject(rhs)
{ }

CPlayer::~CPlayer()
{ }

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    Animation_Setting();

    m_StateCom->Change_State(PLAYERSTATE::IDLE);
    m_StateCom->Change_Dir(PLAYERDIR::LEFT);

    // Transform 테스트
    m_TransformCom->Set_Pos(_vec3(0.f, 0.f, 1.f));

    return S_OK;
}

_int CPlayer::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    Key_Input(timeDelta);

    return exit;
}

void CPlayer::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
}

void CPlayer::Render_GameObject()
{
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    Render_Setting();

    if (m_TextureCom && m_AnimatorCom)
    {
        const wstring& key   = m_AnimatorCom->Get_CurKey();
        _int           frame = m_AnimatorCom->Get_CurFrame();

        m_TextureCom->Set_Texture(key, frame);
    }

    m_BufferCom->Render_Buffer();

    Render_ImGui();
    Render_Reset();

    m_RectColCom->Render(); // Render Reset 이후 호출해야함
}

void CPlayer::Render_Setting()
{
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_GraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_GraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_GraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_GraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_GraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_GraphicDev->SetRenderState(D3DRS_ALPHAREF, 0);
}

void CPlayer::Render_Reset()
{
    m_GraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_GraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::OnBeginOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnBeginOverlap(self, other);

    cout << "Player Hit" << endl;
}

void CPlayer::OnEndOverlap(CCollider* self, CCollider* other)
{
    CGameObject::OnEndOverlap(self, other);
}

HRESULT CPlayer::Add_Component()
{
    // buffer
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom });

    // transform
    m_TransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_TransformCom });

    // texture
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_PLAYER);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_PLAYER, m_TextureCom });

    // Animator
    m_AnimatorCom = CreateProtoComponent<CAnimator>(this, COMPONENTTYPE::ANIMATOR);
    NULL_CHECK_RETURN(m_AnimatorCom, E_FAIL);

    m_AnimatorCom->Set_TextureType(COMPONENTTYPE::TEX_PLAYER);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::ANIMATOR, m_AnimatorCom });

    // RectCol Componet
    m_RectColCom = CreateProtoComponent<CRectCollider>(this, COMPONENTTYPE::RECT_COLL);
    NULL_CHECK_RETURN(m_RectColCom, E_FAIL);
    m_RectColCom->Set_Size(_vec2(2.f, 2.f));

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::RECT_COLL, m_RectColCom });

    m_StateCom = CreateProtoComponent<CState>(this, COMPONENTTYPE::STATE);
    NULL_CHECK_RETURN(m_StateCom, E_FAIL);

    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::STATE, m_StateCom });

    return S_OK;
}

void CPlayer::Animation_Setting()
{
    // 애니메이션 생성
    m_AnimatorCom->Create_Animation(L"PlayerIdle", 150, 0.02f);
    m_AnimatorCom->Create_Animation(L"PlayerRunDown", 19, 0.02f);

    // State -> Animation 연동
    m_StateCom->Set_AnimInfo(PLAYERSTATE::IDLE, L"PlayerIdle", ANIMSTATE::LOOP);
    m_StateCom->Set_AnimInfo(PLAYERSTATE::RUN, L"PlayerRunDown", ANIMSTATE::LOOP);
}

void CPlayer::Key_Input(const _float& timeDelta)
{
    auto inputMgr = CDInputMgr::GetInstance();

    const _float speed = 10.f;

    _vec3 dir    = { 0.f, 0.f, 0.f };
    bool  moving = false;

    // 앞 뒤
    m_TransformCom->Get_Info(INFO_LOOK, &dir);
    if (inputMgr->Get_DIKeyState(DIK_UP) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, speed);
        moving = true;

        m_StateCom->Change_Dir(PLAYERDIR::UP);
    }

    if (inputMgr->Get_DIKeyState(DIK_DOWN) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, -speed);
        moving = true;

        m_StateCom->Change_Dir(PLAYERDIR::DOWN);
    }

    // 좌 우
    m_TransformCom->Get_Info(INFO_RIGHT, &dir);
    if (inputMgr->Get_DIKeyState(DIK_RIGHT) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, speed);
        moving = true;

        m_StateCom->Change_Dir(PLAYERDIR::RIGHT);
    }

    if (inputMgr->Get_DIKeyState(DIK_LEFT) & 0x80)
    {
        D3DXVec3Normalize(&dir, &dir);
        m_TransformCom->Move_Pos(dir, timeDelta, -speed);
        moving = true;

        m_StateCom->Change_Dir(PLAYERDIR::LEFT);
    }

    if (m_StateCom)
    {
        if (moving)
            m_StateCom->Change_State(PLAYERSTATE::RUN);

        else
            m_StateCom->Change_State(PLAYERSTATE::IDLE);
    }
}

void CPlayer::Render_ImGui()
{
    if (ImGui::Begin("Player Inspector"))
    {
        // TransformComponent
        if (m_TransformCom && ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const _vec3& pos = m_TransformCom->Get_Pos();

            ImGui::Text("Position");

            ImGui::Text("X :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosX", (float*)&pos.x);

            ImGui::Text("Y :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosY", (float*)&pos.y);

            ImGui::Text("Z :");
            ImGui::SameLine();
            ImGui::InputFloat("##PlayerPosZ", (float*)&pos.z);

            m_TransformCom->Set_Pos(pos);
        }

        // StateComponent
        if (m_StateCom && ImGui::CollapsingHeader("State Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("State : %s", Engine::ToString(m_StateCom->Get_State()));
            ImGui::Text("Dir   : %s", Engine::ToString(m_StateCom->Get_Dir()));
        }
    }

    ImGui::End();
}

CPlayer* CPlayer::Create(DEVICE graphicDev)
{
    auto player = new CPlayer(graphicDev);

    if (FAILED(player->Ready_GameObject()))
    {
        MSG_BOX("pPlayer Create Failed");
        Safe_Release(player);
        return nullptr;
    }

    return player;
}

void CPlayer::Free()
{
    CGameObject::Free();
}
