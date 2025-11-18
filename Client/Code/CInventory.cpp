#include "pch.h"
#include "CInventory.h"



CInventory::CInventory(DEVICE graphicDev) : CComponent(graphicDev), invenSlotNum(0)
{
}

CInventory::CInventory(const CInventory& rhs) : CComponent(rhs), invenSlotNum(0)
{
}

CInventory::~CInventory()
{
    
}

HRESULT CInventory::Ready_Inventory()
{

    return S_OK;
}

void CInventory::SetInvenSlotNum(int slotNum)
{
    //클론 한 뒤에 무조건 호출 해야합니다.
    invenSlotNum = slotNum;
    m_vecInven.resize(slotNum);
}

void CInventory::DropItem(int DropItemNum, _vec3 DropPos)
{
    int size;

    if (DropItemNum == -1 || m_vecInven.size() <= DropItemNum)// 모든 아이템 드랍
    {
        size = m_vecInven.size();

        for (auto& item : m_vecInven)
        {

        }

    }
    else
    {
        size = DropItemNum;

    }
}

void CInventory::AddItem(int id)
{

}

CInventory* CInventory::Create(DEVICE graphicDev)
{
    auto Inven = new CInventory(graphicDev);

    if (FAILED(Inven->Ready_Inventory()))
    {
        MSG_BOX("Inven Create Failed");
        Safe_Release(Inven);
        return nullptr;
    }

    return Inven;
}

CComponent* CInventory::Clone()
{
    return new CInventory(*this);
}


void CInventory::Free()
{

    CComponent::Free();
}
