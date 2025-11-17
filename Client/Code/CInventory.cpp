#include "pch.h"
#include "CInventory.h"



CInventory::CInventory()
{
}

CInventory::CInventory(const CInventory& rhs)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Inventory()
{
    return S_OK;
}


CInventory* CInventory::Create()
{
    return nullptr;
}

CComponent* CInventory::Clone()
{
    return nullptr;
}


void CInventory::Free()
{
}
