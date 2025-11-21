#pragma once

#include "CComponent.h"

BEGIN(Engine)

class CTransform;
class CTerrainTex;
class CTexture;

END

class CTerrainRenderer : public CComponent
{
private:
    explicit CTerrainRenderer(DEVICE graphicDev);
    explicit CTerrainRenderer(const CTerrainRenderer& rhs);
    virtual  ~CTerrainRenderer();
public:
    HRESULT Ready_TerrainRenderer();
    _int    Update_Component(const _float& timeDelta) override;
    void    LateUpdate_Component() override;
    void    Render_TerrainRenderer();

    virtual void Render_Editor() override;

public:
    HRESULT SaveMap(const char* filePath);
    HRESULT LoadMap(const char* filePath);

public:
    // 셀 좌표 -> 월드 좌표로 변환
    _vec3 GetCellCenterWorld(_uint cellX, _uint cellZ) const;
    void  SelectPlaceObject(_uint cellX, _uint cellZ);

    bool  PickCellMousePos(_int& outX, _int& outZ);

    HRESULT Refresh_MapFileList();


public:
    static CTerrainRenderer* Create(DEVICE graphicDev);
    CComponent* Clone() override;

protected:
    void Free() override;

private:
    CTransform*     m_TransformCom;
    CTerrainTex*    m_BufferCom;
    CTexture*       m_TextureCom;

    // 타일 데이터
    vector<_int>    m_TileIndices;
    _int            m_SelectedTile;
    _int            m_PaintX, m_PaintZ;

    vector<string>  m_MapList;
    char            m_MapName[64];
    _int            m_SelectedMapIndex;
};
