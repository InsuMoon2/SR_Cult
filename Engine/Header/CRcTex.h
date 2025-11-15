#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

	void Set_UV(_float u0, _float v0, _float u1, _float v1);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();
private:
	virtual void Free();

};

END
