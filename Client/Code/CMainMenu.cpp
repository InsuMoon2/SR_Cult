#include "pch.h"
#include "CMainMenu.h"
#include"CLayer.h"
#include "CPersistentObjectHelper.h"
#include "CMainCamera.h"
#include "CGameObject.h"
#include "CMenuChar.h"
#include "CTransform.h"
#include "CUIIcon.h"
#include "CDInputMgr.h"
#include "CStage.h"
#include "CEdit.h"
#include "CSceneMgr.h"

CMainMenu::CMainMenu(DEVICE graphicDev) : Engine::CScene(graphicDev)
{
}

CMainMenu::~CMainMenu()
{
}

HRESULT CMainMenu::Ready_Scene()
{
    if(FAILED(Ready_Environment_Layer(LAYERTYPE::ENVIRONMENT)))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(LAYERTYPE::GAMELOGIC)))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(LAYERTYPE::UI)))
        return E_FAIL;



    return S_OK;
}

_int CMainMenu::Update_Scene(const _float& timeDelta)
{
    _int exit = Engine::CScene::Update_Scene(timeDelta);
    // Stage
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_1) & 0x80)
    {
        
        CSceneMgr::GetInstance()->Change_SceneMgr(SCENETYPE::SC_STAGE);
    }
    // Editor
    else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_2) & 0x80)
    {
        
        CSceneMgr::GetInstance()->Change_SceneMgr(SCENETYPE::SC_EDIT);
    }
    return exit;
}

void CMainMenu::LateUpdate_Scene(const _float& timeDelta)
{
    Engine::CScene::LateUpdate_Scene(timeDelta);
}

void CMainMenu::Render_Scene()
{

}

HRESULT CMainMenu::Ready_Environment_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, layer });          // scene -> map

    return S_OK;
}

HRESULT CMainMenu::Ready_GameLogic_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;


    pGameObject = CMenuChar::Create(m_GraphicDev);
    NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CMainMenu::Ready_GameLogic_Layer() failed: CMenuChar::Create() returned null")

        FAILED_CHECK_MSG(
            layer->Add_GameObject(OBJTYPE::PLAYER, pGameObject),
            L"CMainMenu::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(CMenuChar) failed");

    const auto playerTransform =
        dynamic_cast<CTransform*>(pGameObject->Get_Component(
            ID_DYNAMIC,
            COMPONENTTYPE::TRANSFORM));


    FAILED_CHECK_MSG(
        Engine::AcquirePersistentObject<CMainCamera>(
            OBJTYPE::CAMERA,
            m_GraphicDev,
            layer,
            &pGameObject),
        L"Persistent object setup failed");
   
    // 카메라 타겟을 플레이어로 설정
     dynamic_cast<CMainCamera*>(pGameObject)->Set_CamTarget(playerTransform);
   

     


    if (nullptr == layer)
        return E_FAIL;


    m_Layers.insert({ layerType, layer });          // scene -> map

    return S_OK;
}

HRESULT CMainMenu::Ready_UI_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    //Engine::CGameObject* pGameObject = nullptr;
    CUIIcon* pGameObject = CUIIcon::Create(m_GraphicDev);
    pGameObject->Set_TextureIndex(0);
    pGameObject->Get_Transform()->Set_Scale(262.f,143.f,1.f);
   // pGameObject = CUIIcon::Create(m_GraphicDev);

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::UI, pGameObject),
        L"CMainMenu::Ready_UI_Layer() failed: CLayer::Add_GameObject(CUIIcon) failed");


    m_Layers.insert({ layerType, layer });          // scene -> map

    return S_OK;
}

HRESULT CMainMenu::Ready_Prototype()
{
    return S_OK;
}

CMainMenu* CMainMenu::Create(DEVICE graphicDev)
{
    auto stage = new CMainMenu(graphicDev);

    if (FAILED(stage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(stage);
        return nullptr;
    }

    return stage;
}

void CMainMenu::Free()
{
    Engine::CScene::Free();

}
