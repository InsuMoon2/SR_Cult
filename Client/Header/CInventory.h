#pragma once
#include "CComponent.h"
#include "ItemInstance.h"
#include "Engine_Define.h"

class CInventory :  public CComponent
{
private:
    explicit CInventory();
   
    explicit CInventory(const CInventory& rhs);
    virtual ~CInventory();

public:
    virtual HRESULT		Ready_Inventory();
    //virtual void		Render_Inventory();

public:
    static CInventory* Create();
    virtual CComponent* Clone();
private:
    vector<ItemInstance*> m_vecInven;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

public:
    virtual void	Free();
};

