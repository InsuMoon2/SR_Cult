#include "CResourceManager.h"

IMPLEMENT_SINGLETON(CResourceManager)

CResourceManager::CResourceManager()
{
    // XX 미구현. 필요할지?
}

CResourceManager::~CResourceManager()
{
    CResourceManager::Free();
}

CTexture* CResourceManager::LoadTexture(const wstring& key, const wstring& path, TEXTUREID type, UINT index)
{
    return nullptr;
}

CTexture* CResourceManager::FindTexture(const wstring& key)
{
    return nullptr;
}

void CResourceManager::Free()
{ }
