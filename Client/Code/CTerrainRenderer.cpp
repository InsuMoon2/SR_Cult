#include "pch.h"
#include "CTerrainRenderer.h"
#include "CTransform.h"
#include "CTexture.h"
#include "CDInputMgr.h"
#include "CGameObject.h"
#include "CTerrainTex.h"

static const int ATLAS_COLS_284 = 5;  // 가로 5칸
static const int ATLAS_ROWS_284 = 1;  // 세로 1칸

CTerrainRenderer::CTerrainRenderer(DEVICE graphicDev)
    : CComponent(graphicDev)
{ }

CTerrainRenderer::CTerrainRenderer(const CTerrainRenderer& rhs)
    : CComponent(rhs)
{ }

CTerrainRenderer::~CTerrainRenderer()
{ }

HRESULT CTerrainRenderer::Ready_TerrainRenderer()
{
    if (m_Owner)
    {
        m_TransformCom = dynamic_cast<CTransform*>(
            m_Owner->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM));

        m_BufferCom = dynamic_cast<CTerrainTex*>(
            m_Owner->Get_Component(ID_STATIC, COMPONENTTYPE::TERRAIN_TEX));

        m_TextureCom = dynamic_cast<CTexture*>(
            m_Owner->Get_Component(ID_STATIC, COMPONENTTYPE::TEX_TILE_284));
    }

    NULL_CHECK_RETURN(m_TransformCom, E_FAIL);
    NULL_CHECK_RETURN(m_BufferCom, E_FAIL);
    NULL_CHECK_RETURN(m_TextureCom, E_FAIL);

    const int iCellCntX = TILE_CNT_X - 1;
    const int iCellCntZ = TILE_CNT_Z - 1;

    m_TileIndices.resize(iCellCntX * iCellCntZ, 0);

    // 초기에 맵 리스트 로드 한번 진행
    Refresh_MapFileList();

    return S_OK;
}


_int CTerrainRenderer::Update_Component(const _float& timeDelta)
{
    _int exit = CComponent::Update_Component(timeDelta);

    // ImGui에 마우스 입력 중일 시에는 에디터 입력 무시
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        int cellX, cellZ;
        if (PickCellMousePos(cellX, cellZ))
        {
            if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB) & 0x80)
            {
                m_PaintX = cellX;
                m_PaintZ = cellZ;

                const int iCellCntX = TILE_CNT_X - 1;
                const int index = cellZ * iCellCntX + cellX;

                if (index >= 0 && index < (int)m_TileIndices.size())
                    m_TileIndices[index] = m_SelectedTile;
            }
        }
    }

    return exit;
}

void CTerrainRenderer::LateUpdate_Component()
{
    CComponent::LateUpdate_Component();
}

void CTerrainRenderer::Render_TerrainRenderer()
{
    if (m_BufferCom == nullptr || m_TextureCom == nullptr || m_TransformCom == nullptr)
        return;

    m_GraphicDev->SetTransform(D3DTS_WORLD, &m_TransformCom->Get_World());
    m_GraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_GraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_GraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_GraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    m_GraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    m_GraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    m_BufferCom->ApplyTileSetting(m_TileIndices, ATLAS_COLS_284, ATLAS_ROWS_284);

    m_TextureCom->Set_Texture(0);
    m_BufferCom->Render_Buffer();
}

void CTerrainRenderer::Render_Editor()
{
    CComponent::Render_Editor();

    ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar);

    // 메뉴바
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // --- Save ---
            ImGui::Text(u8"저장할 맵 이름 : ");
            ImGui::InputText("##MapName", m_MapName, IM_ARRAYSIZE(m_MapName));

            if (ImGui::MenuItem(u8"현재 이름으로 저장(Save)", "Ctrl + S"))
            {
                // 풀 경로 만들기
                string fullPath = "../Bin/Resource/Texture/Terrain/";
                fullPath += m_MapName;
                fullPath += ".map";

                if (SUCCEEDED(SaveMap(fullPath.c_str())))
                {
                    // 저장 후 새로고침
                    Refresh_MapFileList();
                }

            }

            ImGui::Separator();

            // --- Load ---
            ImGui::Text(u8"저장된 맵 리스트 : ");

            // 맵 선택
            if (!m_MapList.empty())
            {
                for (_int i = 0; i < (_int)m_MapList.size(); ++i)
                {
                    bool isSelected = (m_SelectedMapIndex == i);

                    if (ImGui::MenuItem(m_MapList[i].c_str(), nullptr, isSelected))
                    {
                        m_SelectedMapIndex = i;

                        // 선택하고 바로 로드
                        string fullPath = "../Bin/Resource/Texture/Terrain/";
                        fullPath += m_MapList[i];

                        LoadMap(fullPath.c_str());
                    }
                }
            }
            else
            {
                ImGui::TextDisabled(u8"[맵 파일 없음])");
            }


            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // 선택된 타일 인덱스 표시용
    ImGui::Text(u8"선택된 타일 : %d", m_SelectedTile + 1);
    ImGui::Separator();

    // 타일 이미지들 보여주기
    if (m_TextureCom)
    {
        const _uint tileCount = 5;
        const _uint tilePerRow = 5;
        const ImVec2 tileSize(32.f, 32.f);

        IDirect3DBaseTexture9* baseTexture = m_TextureCom->Get_BaseTexture(0);

        if (baseTexture)
        {
            ImTextureID texID = (ImTextureID)baseTexture;

            for (_uint i = 0; i < tileCount; i++)
            {
                if (i > 0 && (i % tilePerRow) != 0)
                    ImGui::SameLine();

                // UV 계산
                _int tileX = i;
                _int tileY = 0;

                _float du = 1.f / (float)ATLAS_COLS_284;
                _float dv = 1.f / (float)ATLAS_ROWS_284;

                ImVec2 uv0(tileX * du, tileY * dv);
                ImVec2 uv1((tileX + 1) * du, (tileY + 1) * dv);

                ImGui::PushID(static_cast<int>(i));

                const _bool isSelected = (i == (_uint)m_SelectedTile);

                if (isSelected)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.2f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.3f, 1.0f));
                }

                if (ImGui::ImageButton("Tile", texID, tileSize, uv0, uv1))
                {
                    m_SelectedTile = static_cast<int>(i);
                }

                if (isSelected)
                {
                    ImGui::PopStyleColor(2);
                }

                ImGui::PopID();
            }
        }
    }

    ImGui::Separator();

    if (ImGui::Button(u8"전체 타일을 선택된 타일로"))
    {
        for (size_t i = 0; i < m_TileIndices.size(); ++i)
            m_TileIndices[i] = m_SelectedTile;
    }

    ImGui::End();
}

HRESULT CTerrainRenderer::SaveMap(const char* filePath)
{
    ofstream ofs(filePath, ios_base::binary);
    if (!ofs.is_open())
        return E_FAIL;

    // 간단한 헤더(나중에 버전 바꿀 때 대비)
    uint32_t magic = 0x54494C45; // 'TILE'
    uint32_t version = 1;

    uint32_t cellCntX = TILE_CNT_X - 1;
    uint32_t cellCntZ = TILE_CNT_Z - 1;
    uint32_t tileSize = static_cast<uint32_t>(m_TileIndices.size());

    ofs.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    ofs.write(reinterpret_cast<char*>(&version), sizeof(version));
    ofs.write(reinterpret_cast<char*>(&cellCntX), sizeof(cellCntX));
    ofs.write(reinterpret_cast<char*>(&cellCntZ), sizeof(cellCntZ));

    if (tileSize > 0)
    {
        ofs.write(reinterpret_cast<const char*>(m_TileIndices.data()),
            sizeof(_int) * tileSize);
    }

    ofs.close();

    return S_OK;
}

HRESULT CTerrainRenderer::LoadMap(const char* filePath)
{
    ifstream ifs(filePath, ios_base::binary);
    if (!ifs.is_open())
        return E_FAIL;

    uint32_t magic = 0;
    uint32_t version = 0;
    uint32_t cellCntX = 0;
    uint32_t cellCntZ = 0;

    ifs.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    ifs.read(reinterpret_cast<char*>(&version), sizeof(version));
    ifs.read(reinterpret_cast<char*>(&cellCntX), sizeof(cellCntX));
    ifs.read(reinterpret_cast<char*>(&cellCntZ), sizeof(cellCntZ));

    // 파일 포맷 체크
    if (magic != 0x54494C45)
    {
        ifs.close();
        return E_FAIL;
    }

    // 현재 지형 셀 개수와 다르면 일단 실패 처리
    if (cellCntX != (TILE_CNT_X - 1) || cellCntZ != (TILE_CNT_Z - 1))
    {
        ifs.close();
        return E_FAIL;
    }

    const size_t tileSize = static_cast<size_t>(cellCntX) * cellCntZ;
    m_TileIndices.resize(tileSize);

    if (tileSize > 0)
    {
        ifs.read(reinterpret_cast<char*>(m_TileIndices.data()),
            sizeof(_int) * tileSize);
    }

    ifs.close();
    return S_OK;
}

_vec3 CTerrainRenderer::GetCellCenterWorld(_uint cellX, _uint cellZ) const
{
    const _float vtxItv = 1.f;

    return _vec3(
        cellX * vtxItv + vtxItv * 0.5f,
        0.f,
        cellZ * vtxItv + vtxItv * 0.5f);
}

void CTerrainRenderer::SelectPlaceObject(_uint cellX, _uint cellZ)
{
    const _vec3 pos = GetCellCenterWorld(cellX, cellZ);

    // TODO : 배치할 건물 하나 만들어보기
    //CGameObject* object = 건물 배치?
}

bool CTerrainRenderer::PickCellMousePos(_int& outX, _int& outZ)
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(g_hWnd, &mousePos);

    // 1. 뷰포트 & 클라이언트 크기 보정
    RECT rcClient;
    GetClientRect(g_hWnd, &rcClient);
    D3DVIEWPORT9 viewport;
    m_GraphicDev->GetViewport(&viewport);

    if (rcClient.right > 0 && rcClient.bottom > 0)
    {
        mousePos.x = (long)(mousePos.x * (viewport.Width / (float)rcClient.right));
        mousePos.y = (long)(mousePos.y * (viewport.Height / (float)rcClient.bottom));
    }

    // 2. 투영/뷰 행렬 가져오기
    _matrix matProj, matView;
    m_GraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
    m_GraphicDev->GetTransform(D3DTS_VIEW, &matView);

    // Ray를 World 좌표계 고정으로 변환
    _matrix matIdentity;
    D3DXMatrixIdentity(&matIdentity);

    _vec3 vNear((float)mousePos.x, (float)mousePos.y, 0.0f);
    _vec3 vFar((float)mousePos.x, (float)mousePos.y, 1.0f);

    D3DXVec3Unproject(&vNear, &vNear, &viewport, &matProj, &matView, &matIdentity);
    D3DXVec3Unproject(&vFar, &vFar, &viewport, &matProj, &matView, &matIdentity);

    // 3. 레이(Ray) 생성 (World 기준)
    _vec3 rayOrigin = vNear;
    _vec3 rayDir = vFar - vNear;
    D3DXVec3Normalize(&rayDir, &rayDir);

    // 4. 지형 평면과 충돌 검사 (World 기준)
    _vec3 terrainPos;
    m_TransformCom->Get_Info(INFO_POS, &terrainPos);
    float planeY = terrainPos.y;

    // 평면과 평행하면 무시
    if (fabs(rayDir.y) < 1e-6f) return false;

    // 레이가 평면 높이(planeY)에 도달하는 거리 t 계산
    float t = (planeY - rayOrigin.y) / rayDir.y;

    // 카메라 뒤쪽이면 무시
    if (t < 0.0f) return false;

    // 충돌 지점 (World 좌표)
    _vec3 hitPosWorld = rayOrigin + rayDir * t;

    // 5. World 좌표 -> Local 좌표 변환
    _matrix matWorld = m_TransformCom->Get_World();
    _matrix matInvWorld;
    D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

    _vec3 hitPosLocal;
    D3DXVec3TransformCoord(&hitPosLocal, &hitPosWorld, &matInvWorld);

    // 6. 인덱스 계산
    const float cellSize = (float)VTX_IV;

    // 역행렬을 썼으므로 그냥 나누기
    _int cellX = (int)floorf(hitPosLocal.x / cellSize);
    _int cellZ = (int)floorf(hitPosLocal.z / cellSize);

    // 7. 범위 체크
    if (cellX < 0 || cellX >= (int)TILE_CNT_X - 1 ||
        cellZ < 0 || cellZ >= (int)TILE_CNT_Z - 1)
    {
        return false;
    }

    outX = cellX;
    outZ = cellZ;

    return true;
}

HRESULT CTerrainRenderer::Refresh_MapFileList()
{
    m_MapList.clear();

    const char* mapDir = "../Bin/Resource/Texture/Terrain";

    filesystem::path dirPath(mapDir);
    if (!filesystem::exists(dirPath) || !filesystem::is_directory(dirPath))
        return E_FAIL;

    for (const auto& entry : filesystem::directory_iterator(dirPath))
    {
        if (!entry.is_regular_file())
            continue;

        const filesystem::path& p = entry.path();
        if (p.extension() == ".map")
        {
            // 파일명만 유지 (경로는 제외)
            m_MapList.push_back(p.filename().string());
        }
    }

    // 정렬 한번 후 출력
    sort(m_MapList.begin(), m_MapList.end());

    if (m_MapList.empty())
        m_SelectedMapIndex = -1;

    else if (m_SelectedMapIndex >= (_int)m_MapList.size())
        m_SelectedMapIndex = (_int)m_MapList.size() - 1;

    return S_OK;
}

CTerrainRenderer* CTerrainRenderer::Create(DEVICE graphicDev)
{
    CTerrainRenderer* renderer = new CTerrainRenderer(graphicDev);

    if (renderer == nullptr)
        return nullptr;

    /*if (FAILED(renderer->Ready_TerrainRenderer()))
    {
        Safe_Release(renderer);
        MSG_BOX("Terrain_Renderer Create Faield");
        return nullptr;
    }*/

    return renderer;
}

CComponent* CTerrainRenderer::Clone()
{
    return new CTerrainRenderer(*this);
}

void CTerrainRenderer::Free()
{
    CComponent::Free();
}
