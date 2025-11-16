#include "CUIPanel.h"

CUIPanel::CUIPanel()
{
}

CUIPanel::CUIPanel(DEVICE pGraphicDev)
{
}

CUIPanel::CUIPanel(const CUIPanel& rhs)
{
}

CUIPanel::~CUIPanel()
{
}

_int CUIPanel::Update_Panel(const _float& fTimeDelta)
{
	for (auto& child : m_Children)
	{
		child->Update_UI(fTimeDelta);
	}

	return 0;
}
void CUIPanel::AddChild(CUI* ui)
{
	if (ui == nullptr)
		return;

	m_Children.push_back(ui);
}

void CUIPanel::Free()
{
	
}