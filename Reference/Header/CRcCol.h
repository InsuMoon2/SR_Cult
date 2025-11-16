#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
    explicit CRcCol();
    explicit CRcCol(LPDIRECT3DDEVICE9 graphicDev);
    explicit CRcCol(const CRcCol& rhs);
    ~CRcCol() override;

public:
    HRESULT Ready_Buffer() override;
    void    Render_Buffer() override;

public:
    static CRcCol* Create(LPDIRECT3DDEVICE9 graphicDev);
    CComponent*    Clone() override;

private:
    void Free() override;
};

END
