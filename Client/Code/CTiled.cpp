#include "pch.h"
#include "CTiled.h"
#include "CEnumHelper.h"
#include "CCreateHelper.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CTransform.h"

CTiled::CTiled(DEVICE GraphicDev)
    : CGameObject(GraphicDev)
{
}

CTiled::CTiled(const CTiled& rhs)
    :CGameObject(rhs.m_GraphicDev)
{
}

CTiled::~CTiled()
{
}

HRESULT CTiled::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    // 기본 맵 사이즈
    Set_MapSize(50, 50);
    m_TileWorldSize = 1.f;

    return S_OK;
}

_int CTiled::Update_GameObject(const _float& timeDelta)
{
    _int exit = CGameObject::Update_GameObject(timeDelta);

    return exit;
}

void CTiled::LateUpdate_GameObject(const _float& timeDelta)
{
    CGameObject::LateUpdate_GameObject(timeDelta);

    Render_TileImGui();

}

void CTiled::Render_GameObject()
{

}

void CTiled::Render_TileImGui()
{
    ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar);

    // 메뉴바
    if(ImGui::BeginMenuBar())
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

    ImGui::End();
}

CTiled* CTiled::Create(DEVICE GraphicDev)
{
    CTiled* tile = new CTiled(GraphicDev);

    if (FAILED(tile->Ready_GameObject()))
    {
        Safe_Release(tile);
        MSG_BOX("Create Tile Failed");
        return nullptr;
    }

    return tile;
}

void CTiled::Set_MapSize(_int width, _int height)
{

}

void CTiled::Set_Tile(_int x, _int y, _int index)
{

}

_int CTiled::Get_Tile(_int x, _int y)
{

    return 0;
}

HRESULT CTiled::Add_Component()
{
    m_BufferCom = CreateProtoComponent<CRcTex>(this, COMPONENTTYPE::RC_TEX);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::RC_TEX, m_BufferCom } );

    m_Transform = CreateProtoComponent<CTransform>(this, COMPONENTTYPE::TRANSFORM);
    NULL_CHECK_RETURN(m_Transform, E_FAIL);
    m_Components[ID_DYNAMIC].insert({ COMPONENTTYPE::TRANSFORM, m_Transform });

    m_Texture = CreateProtoComponent<CTexture>(this, COMPONENTTYPE::TEX_TILE);
    NULL_CHECK_RETURN(m_Texture, E_FAIL);
    m_Components[ID_STATIC].insert({ COMPONENTTYPE::TEX_TILE, m_Texture });

   
    return S_OK;
}

void CTiled::Free()
{
    CGameObject::Free();
}
