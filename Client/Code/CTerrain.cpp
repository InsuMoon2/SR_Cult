#include "pch.h"
#include "CTerrain.h"

#include <CRenderer.h>

#include "CEnumHelper.h"
#include "CCreateHelper.h"
#include "CTerrainTex.h"
#include "CTexture.h"
#include "CTransform.h"

CTerrain::CTerrain(DEVICE GraphicDev)
    : CGameObject(GraphicDev)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
    :CGameObject(rhs)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_TransformCom->Set_Pos(_vec3(0.f, -1.f, 0.1f));

    return S_OK;
}

_int CTerrain::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return exit;
}

void CTerrain::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);
    

}

void CTerrain::Render_GameObject()
{
    if (m_TransformCom == nullptr || m_BufferCom == nullptr)
        return;

    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    m_TextureCom->Set_Texture(2);

    m_BufferCom->Render_Buffer();         

    Render_TileImGui();

}

HRESULT CTerrain::Add_Component()
{
    m_BufferCom = CreateProtoComponent<CTerrainTex>(this, COMPONENTTYPE::TERRAIN_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TERRAIN_TEX, m_BufferCom });

    m_TransformCom = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);
    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_TransformCom });

    m_TextureCom = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_TILE);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_TILE, m_TextureCom });

    return S_OK;
}

void CTerrain::Render_TileImGui()
{
    ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar);

    // 메뉴바
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                cout << "Save Clicked" << endl;
            }
            if (ImGui::MenuItem("Load"))
            {
                cout << "Load Clicked" << endl;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // 선택된 타일 인덱스 표시용
    ImGui::Text(u8"선택된 파일 : %d", m_SelectedTile + 1);
    ImGui::Separator();

    // 타일 이미지들 보여주기
    if (m_TextureCom)
    {
        const _uint tileCount = m_TextureCom->Get_TextureIndex();
        //ImGui::Text(u8"로딩된 타일 개수 : %d", tileCount);
        const _uint tilePerRow = 5; // 몇 줄로 보여줄지 ?
        const ImVec2 tileSize(32.f, 32.f);

        for (_uint i = 0; i < tileCount; i++)
        {
            if (i > 0 && (i % tilePerRow) != 0)
                ImGui::SameLine(); // 같은줄에 배치

            IDirect3DBaseTexture9* texture = m_TextureCom->Get_BaseTexture(i);
            //LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_TextureCom->Get_BaseTexture(i);

            if (texture == nullptr)
                continue;

            ImTextureID texID = (ImTextureID)texture;

            ImGui::PushID(static_cast<int>(i));

            // 선택된 타일 색 변경시켜주기
            if (i == static_cast<_uint>(m_SelectedTile))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.2f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.3f, 1.0f));
            }

            if (ImGui::ImageButton("버튼1", texID, tileSize))
            {
                m_SelectedTile = static_cast<int>(i);
            }

            if (i == m_SelectedTile)
            {
                ImGui::PopStyleColor(2);
            }

            ImGui::PopID();
        }

    }

    ImGui::End();
}

CTerrain* CTerrain::Create(DEVICE GraphicDev)
{
    CTerrain* terrain = new CTerrain(GraphicDev);

    if (FAILED(terrain->Ready_GameObject()))
    {
        Safe_Release(terrain);
        MSG_BOX("Create Tile Failed");
        return nullptr;
    }

    return terrain;
}

void CTerrain::Free()
{
    CGameObject::Free();

}
