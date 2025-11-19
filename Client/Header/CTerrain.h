#pragma once

#include "CGameObject.h"

BEGIN(Engine)
class CRcTex;
class CTerrainTex;
class CTransform;
class CTexture;
END

class CTerrain : public CGameObject
{
private:
    explicit CTerrain(DEVICE GraphicDev);
    explicit CTerrain(const CTerrain& rhs);
    virtual ~CTerrain();

public:
    HRESULT Ready_GameObject() override;
    _int    Update_GameObject(const _float& timeDelta) override;
    void    LateUpdate_GameObject(const _float& timeDelta) override;
    void    Render_GameObject() override;

    void    Render_TileImGui();

public:
    // 셀 좌표 -> 월드 좌표로 변환
    _vec3 GetCellCenterWorld(_uint cellX, _uint cellZ) const;
    void SelectPlaceObject(_uint cellX, _uint cellZ);

private:
    HRESULT Add_Component();

public:
    static CTerrain* Create(DEVICE GraphicDev);

protected:
    void Free() override;

private:
    Engine::CTerrainTex*    m_BufferCom;
    Engine::CTransform*     m_TransformCom;
    Engine::CTexture*       m_TextureCom;

    // 현재 선택된 타일
    _int    m_SelectedTile = 0;

    vector<_int> m_TileIndices;

    _int m_PaintX = 0;
    _int m_PaintZ = 0;

};

