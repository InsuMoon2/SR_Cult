#include "CUIPanel.h"

CUIPanel::CUIPanel()
{
}

CUIPanel::CUIPanel(DEVICE graphicDev)
{
}

CUIPanel::CUIPanel(const CUIPanel& rhs)
{
}

CUIPanel::~CUIPanel()
{
}

HRESULT CUIPanel::Ready_GameObject()
{
    return S_OK;
}

_int CUIPanel::Update_GameObject(const _float& timeDelta)
{
	for (auto& child : m_Children)
	{
		child->Update_GameObject(timeDelta);
	}

	return 0;
}

void CUIPanel::LateUpdate_GameObject(const _float& timeDelta)
{
    for (auto& child : m_Children)
    {
        child->LateUpdate_GameObject(timeDelta);
    }
}

void CUIPanel::AddChild(CUI* ui)
{
	if (ui == nullptr)
		return;

	m_Children.push_back(ui);
}

void CUIPanel::Render_GameObject()
{
}

void CUIPanel::Free()
{
	
}
