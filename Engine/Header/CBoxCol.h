#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCol : public CVIBuffer
{
private:
	explicit CBoxCol();
	explicit CBoxCol(DEVICE pGraphicDev);
	explicit CBoxCol(const CBoxCol& rhs);
	virtual ~CBoxCol();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CBoxCol* Create(DEVICE pGraphicDev);
	virtual CComponent* Clone();
private:
	virtual void Free();

};

END
