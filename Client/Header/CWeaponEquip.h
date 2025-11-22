#pragma once
#include "CComponent.h"
#include "CInventory.h"
#include "ItemInstance.h"

struct Item;

class CWeaponEquip : public CComponent
{
private:
    explicit CWeaponEquip(DEVICE graphicDev);
    explicit CWeaponEquip(const CWeaponEquip& rhs);
    ~CWeaponEquip() override;

public:
    void Equip_Weapon(ItemInstance itemInst);
    void Unequip_Weapon();
    void ApplyWeaponStat(Item* item);
    void RemoveWeaponStat();
    bool Get_WeaponEquipped() { return m_WeaponEquipped; }
    int             Get_CurrentWeaponID() { return m_iCurrentWeaponID; }

    static CWeaponEquip* Create(DEVICE graphicDev);
    CComponent*          Clone() override;

protected:
    void Free() override;
public:
    int           m_iCurrentWeaponID;
    InventorySlot m_iCurrentWeaponSlot;
    bool          m_WeaponEquipped;
};
