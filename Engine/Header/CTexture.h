#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CTexture : public CComponent
{
private:
    explicit CTexture(LPDIRECT3DDEVICE9 graphicDev);
    explicit CTexture(const CTexture& rhs);
    ~CTexture() override;

public:
    HRESULT Ready_Texture(TEXTUREID texType, const wstring& filePath, const _uint& count = 1);
    void    Set_Texture(const _uint& index = 0);

private:
    vector<IDirect3DBaseTexture9*> m_Textures;

public:
    static CTexture* Create(LPDIRECT3DDEVICE9 graphicDev,
                            TEXTUREID         texType,
                            const wstring&    filePath,
                            const _uint&      count);
    CComponent* Clone() override;

private:
    void Free() override;
};

END
