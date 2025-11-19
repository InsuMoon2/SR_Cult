#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
    explicit CTerrainTex(DEVICE graphicDev);
    explicit CTerrainTex(const CTerrainTex& rhs);
    virtual  ~CTerrainTex();

public:
    virtual HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
    virtual void Render_Buffer() override;

public:
    const _vec3* Get_VtxPos() { return m_pPos; }

private:
    HANDLE					m_hFile;
    BITMAPFILEHEADER		m_fH;
    BITMAPINFOHEADER		m_iH;

    _vec3* m_pPos;

public:
    static CTerrainTex* Create(LPDIRECT3DDEVICE9 graphicDev,
        const _ulong& dwCntX = VTXCNTX,
        const _ulong& dwCntZ = VTXCNTZ,
        const _ulong& dwVtxItv = VTXITV);

    virtual CComponent* Clone();
private:
    virtual void Free();

};

END
