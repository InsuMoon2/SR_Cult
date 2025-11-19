#pragma once

#include "CGameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CTerrain : public CGameObject
{
private:
    explicit CTerrain(DEVICE GraphicDev);
    explicit CTerrain(const CTerrain& rhs);
    virtual ~CTerrain();

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& timeDelta) override;
    void LateUpdate_GameObject(const _float& timeDelta) override;
    void Render_GameObject() override;
    void Render_TileImGui();

    static CTerrain* Create(DEVICE GraphicDev);

    // 타일 데이터
    void Set_MapSize(_int width, _int height);
    void Set_Tile(_int x, _int y, _int index);
    _int Get_Tile(_int x, _int y);

private:
    HRESULT Add_Component();

protected:
    void Free() override;

private:
    _int m_Width;
    _int m_Height;
    _float m_TileWorldSize;

    vector<_int> m_Tiles;

private:
    CRcTex*     m_BufferCom;
    CTexture*   m_Texture;
    CTransform* m_Transform;

};

