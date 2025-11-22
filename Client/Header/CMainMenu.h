#pragma once
#include "CScene.h"
class CMainMenu :
    public CScene
{

    private:
        explicit CMainMenu(DEVICE graphicDev);
        virtual ~CMainMenu() override;

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
        static CMainMenu* Create(DEVICE graphicDev);

    private:
        void Free() override;


};

