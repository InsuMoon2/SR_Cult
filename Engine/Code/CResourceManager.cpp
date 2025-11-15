#include "CResourceManager.h"

IMPLEMENT_SINGLETON(CResourceManager)

CResourceManager::CResourceManager()
{
    // XX 미구현. 필요할지?
}

CResourceManager::~CResourceManager()
{
	Free();
}

CTexture* CResourceManager::LoadTexture(const wstring& Key, const wstring& Path, TEXTUREID Type, UINT Index)
{
	return nullptr;
}

CTexture* CResourceManager::FindTexture(const wstring& Key)
{
	return nullptr;
}

void CResourceManager::Free()
{

}
