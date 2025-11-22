#pragma once
#include "CComponent.h"
#include "ItemInstance.h"
#include "CInventory.h"

struct Item;

class CWeaponEquip : public CComponent
{
private:
    explicit CWeaponEquip(DEVICE graphicDev);
    explicit CWeaponEquip(const CWeaponEquip& rhs);
    ~CWeaponEquip() override;

public:
    void            Equip_Weapon(ItemInstance itemInst);
    void            Unequip_Weapon();
    void            ApplyWeaponStat(Item* item);
    void             RemoveWeaponStat();
    //int             Get_CurrentWeaponID() { return m_iCurrentWeaponID; }

    static CWeaponEquip* Create(DEVICE graphicDev);
    CComponent* Clone() override;

protected:
    void Free() override;
public:
    int             m_iCurrentWeaponID;
    InventorySlot    m_iCurrentWeaponSlot;
};
