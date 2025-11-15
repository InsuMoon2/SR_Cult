#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer :  public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

	void Set_UV(_float u0, _float v0, _float u1, _float v1);

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

	_ulong						m_dwVtxSize;	// 버텍스 구조체 크기
	_ulong						m_dwVtxCnt;		// 버텍스 개수
	_ulong						m_dwTriCnt;		// 출력하고자하는 삼각형 개수
	_ulong						m_dwFVF;		// 버텍스 포맷을 지정하는 상수 값


	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;

public:
	virtual void Free();
};

END