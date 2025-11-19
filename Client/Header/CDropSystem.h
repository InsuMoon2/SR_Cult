#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "ItemInstance.h"
#include "CScene.h"

class CInventory;

class CDropSystem :
    public CBase
{
    DECLARE_SINGLETON(CDropSystem)
private:
    explicit CDropSystem();
    virtual ~CDropSystem();
public:
    void SpawnDrop(DEVICE pGraphicDev,const ItemInstance& item, const _vec3& worldPos, CScene* nowScene);
    void SpawnAllFromInven(DEVICE pGraphicDev,CInventory& Inven, CScene* nowScene);
private:

    
private:
    virtual void	Free();
};

