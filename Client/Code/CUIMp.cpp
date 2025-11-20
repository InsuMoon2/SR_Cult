#include "pch.h"
#include "CUIMp.h"

#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CTransform.h"

int CUIMp::m_nextID = 0;

CUIMp::CUIMp(DEVICE pGraphicDev) : CUI(pGraphicDev)
{

    m_ID = ++m_nextID;
}

CUIMp::CUIMp(const CUIMp& rhs) : CUI(rhs)
{
    m_ID = ++m_nextID;
}

CUIMp::~CUIMp()
{
}

HRESULT CUIMp::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(_vec3(-550.f, 260.f, 0.f));
    m_TransformCom->Set_Scale(_vec3(54.f, 54.f, 0.f));
    return S_OK;
}

_int CUIMp::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUI::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}

void CUIMp::LateUpdate_GameObject(const _float& timeDelta)
{
    CUI::LateUpdate_GameObject(timeDelta);
}

void CUIMp::Render_GameObject()
{
    CUI::Render_GameObject();

    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());

    m_TextureCom->Set_Texture(0);

    m_BufferCom->Render_Buffer();


    if (ImGui::Begin("Player XP UI"))
    {

        // TransformComponent
        if (m_TransformCom && ImGui::CollapsingHeader(("Gauge UI")), ImGuiTreeNodeFlags_DefaultOpen)
        {
            const _vec3& pos = m_TransformCom->Get_Pos();

            ImGui::Text("Position");
            float itemWidth = 80.0f;
            ImGui::Text("XP X :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##X", (float*)&pos.x);
            ImGui::SameLine();

            ImGui::Text("XP Y :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##Y", (float*)&pos.y);
            ImGui::SameLine();

            ImGui::Text("XP Z :");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("##Z", (float*)&pos.z);


            m_TransformCom->Set_Pos(pos);

            const _vec3& scale = m_TransformCom->Get_Scale();

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("XP ScaleX", (float*)&scale.x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::InputFloat("XP ScaleY", (float*)&scale.y);

            m_TransformCom->Set_Scale(scale);
        }




    }
    ImGui::End();
}

HRESULT CUIMp::Add_Component()
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
    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_UI_COLOR);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_UI_COLOR, m_TextureCom });

    return S_OK;
}

CUIMp* CUIMp::Create(DEVICE graphicDev)
{
    auto circle = new CUIMp(graphicDev);
    if (FAILED(circle->Ready_GameObject()))
    {
        Safe_Release(circle);
        MSG_BOX("UIXp Created Failed");
        return nullptr;
    }
    return circle;
}

void CUIMp::Free()
{

    CUI::Free();
}
