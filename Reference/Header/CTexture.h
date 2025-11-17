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

    HRESULT Add_Texture(const wstring& animKey,
                            TEXTUREID     texType,
                            const wstring& filePathPattern,
                            _uint         count);

    void Set_Texture(const wstring& animKey, _uint frameIndex);

private:
    vector<IDirect3DBaseTexture9*> m_Textures;

    map<wstring, vector<IDirect3DBaseTexture9*>> m_AnimTextures;

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
