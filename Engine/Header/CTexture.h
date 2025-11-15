#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :  public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT	Ready_Texture(TEXTUREID eType, const wstring& pPath, const _uint& iCnt = 1);
	void	Set_Texture(const _uint& iIndex = 0);


private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const wstring& pPath, const _uint& iCnt);
	virtual CComponent* Clone();

private:
	virtual void Free();
};

END

