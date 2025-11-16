#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CComponent;

class ENGINE_DLL CProtoMgr : public CBase
{
    DECLARE_SINGLETON(CProtoMgr)

private:
    CProtoMgr();
    ~CProtoMgr() override;

public:
    // TODO 인수: 이러면 Component밖에 Prototype을 못만드는데, 생각을 좀 더 해봐야 할듯
    // ProtoMgr를 여러개로 쪼갤지, 키값 받아서 쓸지 ?
    HRESULT     Ready_Prototype(COMPONENTTYPE componentType, CComponent* component);
    CComponent* Clone_Prototype(COMPONENTTYPE componentType);

private:
    CComponent* Find_Prototype(COMPONENTTYPE componentType);

private:
    map<COMPONENTTYPE, CComponent*> m_Prototypes;

private:
    void Free() override;
};

END
