#include "pch.h"
#include "CInventory.h"

#include "CItemDB.h"
#include "CPlayer.h"
#include "ItemData.h"

CInventory::CInventory(DEVICE graphicDev)
    : CComponent(graphicDev), invenSlotNum(0)
{}

CInventory::CInventory(const CInventory& rhs)
    : CComponent(rhs), invenSlotNum(0)
{}

CInventory::~CInventory()
{ }

HRESULT CInventory::Ready_Inventory()
{
    return S_OK;
}

void CInventory::SetInvenSlotNum(int slotNum)
{
    //클론 한 뒤에 무조건 호출 해야합니다.

    if (dynamic_cast<CPlayer*>(m_Owner) != nullptr)
    {
        //플레이어인 경우
        invenSlotNum = 12;
        m_vecInven.resize(slotNum);

        for (int i = 0; i < invenSlotNum; i++)
        {
            if (i < invenSlotNum / 2)
                m_vecInven[i].SlotType = ItemType::Material;
            else
                m_vecInven[i].SlotType = ItemType::FoodMaterial;
        }
    }
    else
    {
        //플레이어가 아닌경우 ex)몬스터
        invenSlotNum = slotNum;
        m_vecInven.resize(slotNum);

        for (int i = 0; i < invenSlotNum; i++)
        {
            m_vecInven[i].SlotType = ItemType::None;
        }
    }
}

void CInventory::DropItem(int DropItemNum, _vec3 DropPos)
{
    int size;

    if (DropItemNum == -1 || m_vecInven.size() <= DropItemNum)// 모든 아이템 드랍
    {
        size = m_vecInven.size();

        for (auto& item : m_vecInven)
        { }
    }
    else
        size = DropItemNum;
}

bool CInventory::AddItem(ItemInstance itemInst)
{
    //ItemInstance의 id(= itemId)는 “인스턴스를 만들 때 즉시 넣어준다.”

    Item* itemDate = CItemDB::GetInstance()->GetItemById(itemInst.itemId);

    // 스택가능한지
    for (auto& slot : m_vecInven)
    {
        if (slot.itemInst.itemId == itemInst.itemId && itemDate->stackable == true)
        {
            if (itemDate->maxStack > slot.count)
            {
                slot.count++;
                return true;
            }
        }
    }

    if (dynamic_cast<CPlayer*>(m_Owner) != nullptr)
    {
        int slot;
        // 스택불가, 같은 아이템 없다
        switch (itemDate->type)
        {
        case ItemType::FoodMaterial:
            slot = FindEmptySlot(ItemType::FoodMaterial);
            if (slot == -1)
                break;
            else
            {
                m_vecInven[slot].itemInst = itemInst;
                return true;
            }
            break;
        case ItemType::Food:
            // 음식은 노예용 플레이어는 소지 안함 먹을순 있는데 플레이어에서 처리
            return false;
            break;
        case ItemType::Material:
            slot = FindEmptySlot(ItemType::Material);
            if (slot == -1)
                break;
            else
            {
                m_vecInven[slot].itemInst = itemInst;
                return true;
            }
            break;
        case ItemType::Weapon:
            // 이건 플레이어에서 바로 처리하자 바로 장착이니까
            return false;
            break;
        }
    }
    else
    {
        int slot = FindEmptySlot(ItemType::None);
        if (slot != -1)
        {
            m_vecInven[slot].itemInst = itemInst;
            return true;
        }
    }
    return false;
}

int CInventory::FindEmptySlot(ItemType itemtype)
{
    if (m_vecInven.empty()) // 인밴토리 칸이 하나도 없는경우
        return -1;

    for (size_t i = 0; i < m_vecInven.size(); ++i)
    {
        if (m_vecInven[i].SlotType == itemtype)
        {
            if (m_vecInven[i].itemInst.itemId == -1)
                return static_cast<int>(i);
        }
    }

    return -1;
}

bool CInventory::RemoveItem(int itemID, int count)
{
    int remainCount = count;
    for (auto& slot : m_vecInven)
    {
        if (itemID == slot.itemInst.itemId)
        {
            if (slot.count <= remainCount)
            {
                slot.count           = 0;
                slot.itemInst.itemId = -1;

                if (remainCount == 0)
                    return true;
            }
            else //if (slot.count > count)
            {
                slot.count -= remainCount;
                remainCount = 0;
                return true;
            }
        }
    }

    return false;
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
