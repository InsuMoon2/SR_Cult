#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
    explicit CRcTex();
    explicit CRcTex(LPDIRECT3DDEVICE9 graphicDev);
    explicit CRcTex(const CRcTex& rhs);
    ~CRcTex() override;

public:
    HRESULT Ready_Buffer() override;
    void    Render_Buffer() override;

    void Set_UV(_float u0, _float v0, _float u1, _float v1);

public:
    static CRcTex* Create(LPDIRECT3DDEVICE9 graphicDev);
    CComponent*    Clone() override;
private:
    void Free() override;
};

END
