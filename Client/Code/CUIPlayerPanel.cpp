#include "pch.h"
#include "CUIPlayerPanel.h"
#include "CUIHeartBar.h"
#include "CRenderer.h"
#include "CUIPanel.h"

CUIPlayerPanel::CUIPlayerPanel(DEVICE graphicDev):CUIPanel(graphicDev)
{
}

CUIPlayerPanel::CUIPlayerPanel(const CUIPlayerPanel& rhs):CUIPanel(rhs)
{
}

CUIPlayerPanel::~CUIPlayerPanel()
{
}

HRESULT CUIPlayerPanel::Ready_GameObject()
{
    CUIPanel::Ready_GameObject();

    auto image = CUIHeartBar::Create(m_GraphicDev);
    NULL_CHECK_RETURN(image, E_FAIL)

        
    AddChild(image);

    return S_OK;
}

_int CUIPlayerPanel::Update_GameObject(const _float& timeDelta)
{
    _int exit = CUIPanel::Update_GameObject(timeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return exit;
}
 
void CUIPlayerPanel::LateUpdate_GameObject(const _float& timeDelta)
{
    CUIPanel::LateUpdate_GameObject(timeDelta);

}

void CUIPlayerPanel::Render_GameObject()
{
    CUIPanel::Render_GameObject();

    /*if (ImGui::Begin("Player Panel UI"))
    {

        const float Hp = m_player->m_Hp;
        ImGui::Text("HP :");
        ImGui::SameLine();
        ImGui::InputFloat("##", (float*)&Hp);
    }*/

    //ImGui::End();
}

CUIPlayerPanel* CUIPlayerPanel::Create(DEVICE graphicDev)
{
    auto mainmenupanel = new CUIPlayerPanel(graphicDev);

    if (FAILED(mainmenupanel->Ready_GameObject()))
    {
        MSG_BOX("MainMenu Create Failed");
        Safe_Release(mainmenupanel);
        return nullptr;
    }

    return mainmenupanel;
}

void CUIPlayerPanel::Free()
{
    Engine::CUIPanel::Free();
}

