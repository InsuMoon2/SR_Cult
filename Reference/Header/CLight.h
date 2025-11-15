#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CLight : public CBase
{
private:
    explicit CLight(LPDIRECT3DDEVICE9 graphicDev);
    ~CLight() override;

public:
    HRESULT Ready_Light(const D3DLIGHT9* lightInfo, const _uint& index);

private:
    LPDIRECT3DDEVICE9 m_GraphicDev;
    D3DLIGHT9         m_Light;
    _uint             m_Index;

public:
    static CLight* Create(LPDIRECT3DDEVICE9 graphicDev,
                          const D3DLIGHT9*  lightInfo,
                          const _uint&      index);
private:
    void Free() override;
};

END
