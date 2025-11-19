#include "pch.h"
#include "CUIHeartBar.h"
#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CTransform.h"

CUIHeartBar::CUIHeartBar(DEVICE graphicDev)
    : CUI(graphicDev), m_BufferCom(nullptr), m_TextureCom(nullptr),
      m_TransformCom(nullptr)

{}

CUIHeartBar::CUIHeartBar(const CUIHeartBar& rhs)
    : CUI(rhs), m_BufferCom(nullptr), m_TextureCom(nullptr),
      m_TransformCom(nullptr)
{}

CUIHeartBar::~CUIHeartBar()
{}

HRESULT CUIHeartBar::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(_vec3(-500.f, 300.f, 0.f));
    m_TransformCom->Set_Scale(_vec3(16.f, 16.f, 1.f));

    return S_OK;
}

_int CUIHeartBar::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUI::Update_GameObject(timeDelta);

    //Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUIHeartBar::LateUpdate_GameObject(const _float& timeDelta)
{
    CUI::LateUpdate_GameObject(timeDelta);
}

void CUIHeartBar::Render_GameObject()
{
    // 월드 행렬 세팅
    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    m_TextureCom->Set_Texture(1);
    m_BufferCom->Render_Buffer();

    if (ImGui::Begin("UI_Heart Inspector"))
    {
        // TransformComponent
        if (m_TransformCom && ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const _vec3& pos = m_TransformCom->Get_Pos();

            ImGui::Text("Position");

            ImGui::Text("X :");
            ImGui::SameLine();
            ImGui::InputFloat("##HeartX", (float*)&pos.x);

            ImGui::Text("Y :");
            ImGui::SameLine();
            ImGui::InputFloat("##HeartY", (float*)&pos.y);

            ImGui::Text("Z :");
            ImGui::SameLine();
            ImGui::InputFloat("##HeartZ", (float*)&pos.z);

            m_TransformCom->Set_Pos(pos);

            _vec3 scale = m_TransformCom->Get_Scale();
            ImGui::InputFloat("ScaleX", &scale.x);
            ImGui::InputFloat("ScaleY", &scale.y);
            m_TransformCom->Set_Scale(scale);
        }
    }
    ImGui::End();
}

HRESULT CUIHeartBar::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_HEART);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_HEART, m_TextureCom });

    return S_OK;
}

CUIHeartBar* CUIHeartBar::Create(DEVICE graphicDev)
{
    auto heartbar = new CUIHeartBar(graphicDev);
    if (FAILED(heartbar->Ready_GameObject()))
    {
        Safe_Release(heartbar);
        MSG_BOX("UIHeartBar Created Failed");
        return nullptr;
    }

    return heartbar;
}

void CUIHeartBar::Free()
{
    Safe_Release(m_BufferCom);
    Safe_Release(m_TextureCom);

    CUI::Free();
}
