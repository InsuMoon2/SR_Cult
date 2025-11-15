#pragma once
#include "CTriCol.h"
#include "CRcCol.h"
#include "CRcTex.h"

#include "CTransform.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr :  public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	CProtoMgr();
	virtual ~CProtoMgr();

public:
    // 이러면 Component밖에 Prototype을 못만드는데, 생각을 좀 더 해봐야 할듯
    // ProtoMgr를 여러개로 쪼갤지, 키값 받아서 쓸지 ?
	HRESULT			Ready_Prototype(COMPONENTTYPE eComponentType, CComponent* pComponent);
	CComponent*     Clone_Prototype(COMPONENTTYPE eComponentType);

private:
	CComponent* Find_Prototype(COMPONENTTYPE eComponentType);

private:
	map<COMPONENTTYPE, CComponent*>		m_mapPrototype;

private:
	virtual void Free();

};

END
