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

    void Render_TileImGui();

private:
    HRESULT Add_Component();

private:
    Engine::CTerrainTex*    m_BufferCom;
    Engine::CTransform*     m_TransformCom;
    Engine::CTexture*       m_TextureCom;

    _int    m_SelectedTile = 0;

public:
    static CTerrain* Create(DEVICE GraphicDev);

protected:
    void Free() override;

};

