#pragma once
#include "CComponent.h"
#include "ItemInstance.h"
#include "Engine_Define.h"

enum SlotType
{
   FoodMaterial,
   material,
   Food,
   Weapon
};


struct InventorySlot
{
    SlotType type;        // Material, Food, Weapon…
    ItemInstance item;    // 들어 있는 아이템 (없으면 id = -1)
};

class CInventory :  public CComponent
{
private:
    explicit CInventory(DEVICE graphicDev);
   
    explicit CInventory(const CInventory& rhs);
    virtual ~CInventory();

public:
    virtual HRESULT		Ready_Inventory();
    void SetInvenSlotNum(int slotNum); //무조건 만들어야함 

    void DropItem(int DropItemNum, _vec3 DropPos);

   // void DropItem(int DropItemNum); // 오너가 죽엇을떄 이거 호출하면 드롭 아이템으로 -1 이면 전부
    void AddItem(int id);

public:
    static CInventory* Create(DEVICE graphicDev);
    virtual CComponent* Clone();
private:
    vector<ItemInstance> m_vecInven;
    int invenSlotNum;

public:
    virtual void	Free();
};

