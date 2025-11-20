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
    void SpawnDrop(DEVICE pGraphicDev,const ItemInstance& item, const _vec3& worldPos);
    void SpawnAllFromInven(DEVICE pGraphicDev,CInventory& Inven);

    void SetCurrentScene(CScene* nowScene) { m_nowScene = nowScene; } // 이거 무조건 씬 셋팅 시 해줘야함
private:
    CScene* m_nowScene = nullptr; 
    
private:
    virtual void	Free();
};

