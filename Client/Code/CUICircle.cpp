#include "pch.h"
#include "CUICircle.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CTransform.h"
#include "CCreateHelper.h"
#include "CRenderer.h"

int CUICircle::m_nextID = 0;

CUICircle::CUICircle(DEVICE pGraphicDev) : CUI(pGraphicDev)
{
    m_ID = ++m_nextID;
}

CUICircle::CUICircle(const CUICircle& rhs) : CUI(rhs)
{
    m_ID = ++m_nextID;
}

CUICircle::~CUICircle()
{
}

HRESULT CUICircle::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(_vec3(-550.f, 270.f, 0.f));
    m_TransformCom->Set_Scale(_vec3(95.f, 90.f, 0.f));
    return S_OK;
}

_int CUICircle::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUI::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUICircle::LateUpdate_GameObject(const _float& timeDelta)
{
    CUI::LateUpdate_GameObject(timeDelta);
}

void CUICircle::Render_GameObject()
{
    CUI::Render_GameObject();

    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    m_TextureCom->Set_Texture(0);

    m_BufferCom->Render_Buffer();


    if (ImGui::Begin("Player UI"))
    {

        // TransformComponent
        if (m_TransformCom && ImGui::CollapsingHeader(("Circle")), ImGuiTreeNodeFlags_DefaultOpen)
        {
            const _vec3& pos = m_TransformCom->Get_Pos();

            ImGui::Text("Circle Position");
            float itemWidth = 80.0f;
            ImGui::Text("X :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##Circle X", (float*)&pos.x);
            ImGui::SameLine();

            ImGui::Text("Y :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##Circle Y", (float*)&pos.y);
            ImGui::SameLine();

            ImGui::Text("Z :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##Circle Z", (float*)&pos.z);


            m_TransformCom->Set_Pos(pos);

            const _vec3& scale = m_TransformCom->Get_Scale();

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("Circle ScaleX", (float*)&scale.x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("Circle ScaleY", (float*)&scale.y);

            m_TransformCom->Set_Scale(scale);
        }

    }
    ImGui::End();
}

HRESULT CUICircle::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_CIRCLE);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_CIRCLE, m_TextureCom });

    return S_OK;
}

CUICircle* CUICircle::Create(DEVICE graphicDev)
{
    auto circle = new CUICircle(graphicDev);
    if (FAILED(circle->Ready_GameObject()))
    {
        Safe_Release(circle);
        MSG_BOX("UIHeartBar Created Failed");
        return nullptr;
    }
    return circle;
}

void CUICircle::Free()
{
 
    CUI::Free();
}
