#pragma once
#include "CProtoMgr.h"

BEGIN(Engine)
class CGameObject;

template <typename T>
static T* CreateProtoComponent(CGameObject* Owner, COMPONENTTYPE eComponentType)
{
    if (Owner == nullptr)
        return nullptr;

    auto proto = Engine::CProtoMgr::GetInstance()->Clone_Prototype(eComponentType);
    if (proto == nullptr)
        return nullptr;

    T* component = dynamic_cast<T*>(proto);

    if (component == nullptr)
    {
        Safe_Release(proto);
        return nullptr;
    }

    component->Set_Owner(Owner);

    return component;
}

END
