#include "CRcTex.h"

CRcTex::CRcTex()
{
}

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	//m_dwVtxCnt = 6;
	m_dwVtxCnt = 4;	
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertex = NULL;
		

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = {0.f, 0.f};

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = {1.f, 0.f};

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = {1.f, 1.f};

	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();


	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CRcTex::Set_UV(_float u0, _float v0, _float u1, _float v1)
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 좌상단
	pVertex[0].vTexUV.x = u0;
	pVertex[0].vTexUV.y = v0;

	// 우상단
	pVertex[1].vTexUV.x = u1;
	pVertex[1].vTexUV.y = v0;

	// 우하단
	pVertex[2].vTexUV.x = u1;
	pVertex[2].vTexUV.y = v1;

	// 좌하단
	pVertex[3].vTexUV.x = u0;
	pVertex[3].vTexUV.y = v1;

	m_pVB->Unlock();
}

CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex* pRcTex = new CRcTex(pGraphicDev);

	if (FAILED(pRcTex->Ready_Buffer()))
	{
		Safe_Release(pRcTex);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pRcTex;
}

CComponent* CRcTex::Clone()
{
	return new CRcTex(*this);
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
