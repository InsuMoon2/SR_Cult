#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL CTexture : public CComponent
{
private:
    explicit CTexture(DEVICE graphicDev);
    explicit CTexture(const CTexture& rhs);
    ~CTexture() override;

public:
    HRESULT Ready_Texture(TEXTUREID texType, const wstring& filePath, const _uint& count = 1);
    void    Set_Texture(const _uint& index = 0);

    HRESULT Add_Texture(TEXTUREID texType, const wstring& key, const wstring& filePath);
    void    Set_Texture(const wstring& key);

private:
    vector<IDirect3DBaseTexture9*> m_Textures;

    map<wstring, _uint>            m_KeyIndexMap;

public:
    static CTexture* Create(DEVICE graphicDev,
                            TEXTUREID         texType,
                            const wstring&    filePath,
                            const _uint&      count);
    CComponent* Clone() override;

private:
    void Free() override;
};

END
