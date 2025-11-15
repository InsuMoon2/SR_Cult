#pragma once

#include "CBase.h"
#include "CGameObject.h"
#include "Engine_Define.h"

class CTexture;

BEGIN(Engine)

class ENGINE_DLL CResourceManager : public CBase
{
	DECLARE_SINGLETON(CResourceManager)

private:
	explicit CResourceManager();
	virtual ~CResourceManager();

public:
	CTexture* LoadTexture(const wstring& Key, const wstring& Path, TEXTUREID Type, UINT Index);
	CTexture* FindTexture(const wstring& Key);

private:
	virtual void Free() override;

private:
	map<wstring, CTexture*> m_Textures;

};

END