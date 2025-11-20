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

void CDropSystem::SpawnDrop(DEVICE pGraphicDev,const ItemInstance& itemInst, const _vec3& worldPos)
{
    CDroppedItem* pDrop = CDroppedItem::Create(pGraphicDev, itemInst, worldPos);
    dynamic_cast<CStage*>(m_nowScene)->AddObjectOnLayer(LAYERTYPE::GAMELOGIC, pDrop, OBJTYPE::ITEM);
}

void CDropSystem::SpawnAllFromInven(DEVICE pGraphicDev,CInventory& Inven)
{
    CGameObject* obj = Inven.Get_Owner();
    if (obj == nullptr)
        return;
  
    _vec3 objPos = obj->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM)->Get_Transform()->Get_Pos();
   
    for (int i = 0; i < Inven.GetVector().size();i++)
    {
        const InventorySlot& slot = Inven.GetVector()[i];
        ItemInstance itemInst = slot.itemInst;

        if (itemInst.itemId != -1 && Inven.GetVector()[i].count >= 1)
        {
            int dropCount = slot.count;
            for (int j = 0; j < dropCount; j++)
            {
                SpawnDrop(pGraphicDev,itemInst, objPos);
                Inven.RemoveItem(itemInst.itemId, 1);
            }
        }
    }
}

void CDropSystem::Free()
{
}
