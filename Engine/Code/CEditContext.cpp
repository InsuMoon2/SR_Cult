#include "CEditContext.h"

CEditContext::CEditContext()
    : m_ActiveScene(nullptr), m_SelectedObject(nullptr), m_SelectedComponent(nullptr)
{

}

CEditContext::~CEditContext()
{
    
}

void CEditContext::SelectObject(CGameObject* obj)
{
    m_SelectedObject = obj;
    m_SelectedComponent = nullptr;
}

void CEditContext::SelectComponent(CComponent* comp)
{
    m_SelectedComponent = comp;
}

CEditContext* CEditContext::Create()
{
    CEditContext* ctx = new CEditContext();

    if (ctx == nullptr)
        return nullptr;

    return ctx;
}

void CEditContext::Free()
{

}
