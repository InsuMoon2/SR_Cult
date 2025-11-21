#pragma once
#include "CComponent.h"
#include "ItemInstance.h"


class CWeaponEquip : public CComponent
{
private:
    explicit CWeaponEquip(DEVICE graphicDev);
    explicit CWeaponEquip(const CWeaponEquip& rhs);
    ~CWeaponEquip() override;

public:
    void            Equip_Weapon(ItemInstance itemInst);
    void            Unequip_Weapon();

    int             Get_CurrentWeaponID() { return m_iCurrentWeaponID; }

public:
    int     m_iCurrentWeaponID;

};

