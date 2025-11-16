#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CTexture;

class ENGINE_DLL CResourceManager : public CBase
{
    DECLARE_SINGLETON(CResourceManager)

private:
    explicit CResourceManager();
    ~CResourceManager() override;

public:
    CTexture* LoadTexture(const wstring& key, const wstring& path, TEXTUREID type, UINT index);
    CTexture* FindTexture(const wstring& key);

private:
    void Free() override;

private:
    map<wstring, CTexture*> m_Textures;
};

END
