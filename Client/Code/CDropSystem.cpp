#include "pch.h"
#include "CDropSystem.h"
#include "CInventory.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CDroppedItem.h"
#include "CManagement.h"
#include "CStage.h"

IMPLEMENT_SINGLETON(CDropSystem)

CDropSystem::CDropSystem()
{
}

CDropSystem::~CDropSystem()
{
    Free();
}

void CDropSystem::SpawnDrop(DEVICE pGraphicDev,const ItemInstance& itemInst, const _vec3& worldPos, CScene* nowScene)
{
    CDroppedItem* pDrop = CDroppedItem::Create(pGraphicDev, itemInst, worldPos);
    dynamic_cast<CStage*>(nowScene)->AddObjectOnLayer(LAYERTYPE::GAMELOGIC, pDrop, OBJTYPE::ITEM);
}

void CDropSystem::SpawnAllFromInven(DEVICE pGraphicDev,CInventory& Inven, CScene* nowScene)
{
    CGameObject* obj = Inven.Get_Owner();
    if (obj == nullptr)
        return;
  
    _vec3 objPos = obj->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM)->Get_Transform()->Get_Pos();
   
    for (int i = 0; i < Inven.GetVector().size();i++)
    {
        ItemInstance itemInst = Inven.GetVector()[i].itemInst;
        if (itemInst.itemId != -1 && Inven.GetVector()[i].count >= 1)
        {
            for (int i = 0; i < Inven.GetVector()[i].count; i++)
            {
                SpawnDrop(pGraphicDev,itemInst, objPos, nowScene);
            }
        }
    }
}

void CDropSystem::Free()
{
}
