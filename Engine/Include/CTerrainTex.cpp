#include "CTerrainTex.h"

CTerrainTex::CTerrainTex(DEVICE graphicDev)
    : CVIBuffer(graphicDev), m_hFile(nullptr), m_pPos(nullptr)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
    : CVIBuffer(rhs), m_hFile(rhs.m_hFile), m_fH(rhs.m_fH), m_iH(rhs.m_iH)
    , m_pPos(rhs.m_pPos)
{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX,
    const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    m_VtxSize = sizeof(VTXTEX);
    m_VtxCnt = dwCntX * dwCntZ;
    m_TriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
    m_FVF = FVF_TEX;

    m_pPos = new _vec3[m_VtxCnt];

    m_IdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX* pVertex = NULL;

    _ulong	dwIndex = 0;

    m_VB->Lock(0, 0, (void**)&pVertex, 0);

    for (_ulong i = 0; i < dwCntZ; ++i)
    {
        for (_ulong j = 0; j < dwCntX; ++j)
        {
            dwIndex = i * dwCntX + j;

            pVertex[dwIndex].position = { _float(j * dwVtxItv),
                0.f,
                _float(i * dwVtxItv) };

            pVertex[dwIndex].texUV = {
                                        (_float)j / (dwCntX - 1) * 20.f,
                                        (_float)i / (dwCntZ - 1) * 20.f
            };

            pVertex[dwIndex].normal = { 0.f, 0.f, 0.f };

            m_pPos[dwIndex] = pVertex[dwIndex].position;
        }
    }

    INDEX32* pIndex = NULL;

    _ulong	dwTriCnt = 0;

    _vec3 vNormal, vDst, vSrc;

    m_IB->Lock(0, 0, (void**)&pIndex, 0);

    for (_ulong i = 0; i < dwCntZ - 1; ++i)
    {
        for (_ulong j = 0; j < dwCntX - 1; ++j)
        {
            dwIndex = i * dwCntX + j;

            // 오른쪽 위
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
            pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
            pIndex[dwTriCnt]._2 = dwIndex + 1;

            vDst = pVertex[pIndex[dwTriCnt]._1].position - pVertex[pIndex[dwTriCnt]._0].position;
            vSrc = pVertex[pIndex[dwTriCnt]._2].position - pVertex[pIndex[dwTriCnt]._1].position;

            D3DXVec3Cross(&vNormal, &vDst, &vSrc);

            pVertex[pIndex[dwTriCnt]._0].normal += vNormal;
            pVertex[pIndex[dwTriCnt]._1].normal += vNormal;
            pVertex[pIndex[dwTriCnt]._2].normal += vNormal;

            dwTriCnt++;

            // 왼쪽 아래
            pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
            pIndex[dwTriCnt]._1 = dwIndex + 1;
            pIndex[dwTriCnt]._2 = dwIndex;

            vDst = pVertex[pIndex[dwTriCnt]._1].position - pVertex[pIndex[dwTriCnt]._0].position;
            vSrc = pVertex[pIndex[dwTriCnt]._2].position - pVertex[pIndex[dwTriCnt]._1].position;

            D3DXVec3Cross(&vNormal, &vDst, &vSrc);

            pVertex[pIndex[dwTriCnt]._0].normal += vNormal;
            pVertex[pIndex[dwTriCnt]._1].normal += vNormal;
            pVertex[pIndex[dwTriCnt]._2].normal += vNormal;

            dwTriCnt++;
        }
    }

    for (_uint i = 0; i < m_VtxCnt; ++i)
    {
        D3DXVec3Normalize(&pVertex[i].normal, &pVertex[i].normal);
    }

    m_VB->Unlock();
    m_IB->Unlock();


    return S_OK;
}

void CTerrainTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();


}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ,
    const _ulong& dwVtxItv)
{
}

CComponent* CTerrainTex::Clone()
{
}

void CTerrainTex::Free()
{
    CVIBuffer::Free();
}
