#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CUIPlayerPanel.h"

class CStage : public CScene
{ 
private:
    explicit CStage(DEVICE graphicDev);
    ~CStage() override;

public:
    HRESULT Ready_Scene() override;
    _int    Update_Scene(const _float& timeDelta) override;
    void    LateUpdate_Scene(const _float& timeDelta) override;
    void    Render_Scene() override;

private:
    HRESULT Ready_Environment_Layer(LAYERTYPE layerType);
    HRESULT Ready_GameLogic_Layer(LAYERTYPE layerType);
    HRESULT Ready_UI_Layer(LAYERTYPE layerType);

    HRESULT Ready_Prototype();

public:
    static CStage* Create(DEVICE graphicDev);


private:
    void Free() override;

private:
    CUIPlayerPanel* m_playerPanel;
    CPlayer* m_player;
};
