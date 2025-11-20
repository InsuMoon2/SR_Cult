#include "pch.h"
#include "CStage.h"
#include "CLayer.h"
#include "CMainCamera.h"
#include "CPersistentObjectHelper.h"
#include "CPlayer.h"
#include "CTestMonster.h"
#include "CTransform.h"
#include "CUIPlayerPanel.h"
#include "CHumanMonster.h"
#include "CDropSystem.h"
#include "CUICircle.h"
#include "CUIMp.h"

CStage::CStage(DEVICE graphicDev)
    : Engine::CScene(graphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
    // 로딩 스레드로 역할 이동
    //if (FAILED(Ready_Prototype()))
    //    return E_FAIL;


    if (FAILED(Ready_Environment_Layer(LAYERTYPE::ENVIRONMENT)))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(LAYERTYPE::GAMELOGIC)))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(LAYERTYPE::UI)))
        return E_FAIL;
    // 안은수 test
    CDropSystem::GetInstance()->SetCurrentScene(this);


    ItemInstance i = { 1001,-1,0 };
    CDropSystem::GetInstance()->SpawnDrop(m_GraphicDev, i, { 10,0,10 });

    return S_OK;
}

_int CStage::Update_Scene(const _float& timeDelta)
{
    _int exit = Engine::CScene::Update_Scene(timeDelta);

    return exit;
}

void CStage::LateUpdate_Scene(const _float& timeDelta)
{
    Engine::CScene::LateUpdate_Scene(timeDelta);
}

void CStage::Render_Scene()
{
    // debug 용
}

//void CStage::AddObjectOnLayer(LAYERTYPE layerType, CGameObject* obj, OBJTYPE objType)
//{
//    Engine::CLayer* layer = nullptr;
//
//    auto it = m_Layers.find(layerType);
//    if (it != m_Layers.end())
//    {
//        layer = it->second;
//    }
//
//    if(layer == nullptr)
//      MSG_BOX("layerType wrong");
//
//    if (FAILED(layer->Add_GameObject(objType, obj)))
//    {
//        MSG_BOX("Add_GameObject failed");
//        return;
//    }
//
//}

HRESULT CStage::Ready_Environment_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, layer });          // scene -> map

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    NULL_CHECK_RETURN_MSG(
        layer,
        E_FAIL,
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Create() returned null");

    Engine::CGameObject* gameObject = nullptr;

    // -----------------------------
    // Player
    // -----------------------------

    // 플레이어 생성
    FAILED_CHECK_MSG(
        Engine::AcquirePersistentObject<CPlayer>(
            OBJTYPE::PLAYER,
            m_GraphicDev,
            layer,
            &gameObject),
        L"Persistent object setup failed");

    m_Player = dynamic_cast<CPlayer*>(gameObject);

    // 플레이어의 Transform 가져오기
    const auto playerTransform =
        dynamic_cast<CTransform*>(gameObject->Get_Component(
            ID_DYNAMIC,
            COMPONENTTYPE::TRANSFORM));

    // -----------------------------
    // Camera
    // -----------------------------

    // 카메라 생성
    FAILED_CHECK_MSG(
        Engine::AcquirePersistentObject<CMainCamera>(
            OBJTYPE::CAMERA,
            m_GraphicDev,
            layer,
            &gameObject),
        L"Persistent object setup failed");

    // 카메라 타겟을 플레이어로 설정
    dynamic_cast<CMainCamera*>(gameObject)->Set_CamTarget(playerTransform);

    // -----------------------------
    // Monster
    // -----------------------------

    // monster
    //gameObject = CMonster::Create(m_GraphicDev);
    //
    //if (nullptr == gameObject)
    //    return E_FAIL;
    //
    //if (FAILED(layer->Add_GameObject(OBJTYPE::MONSTER, gameObject)))
    //    return E_FAIL;
    //
    //testMonster
    //gameObject = CTestMonster::Create(m_GraphicDev);

    //NULL_CHECK_RETURN_MSG(
    //    gameObject,
    //    E_FAIL,
    //    L"CStage::Ready_GameLogic_Layer() failed: CMonster::Create() returned null");

    //FAILED_CHECK_MSG(
    //    layer->Add_GameObject(OBJTYPE::BOSS2, gameObject),
    //    L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(BOSS2) failed");

    //HumanMonster
    //gameObject = CHumanMonster::Create(m_GraphicDev);
    //NULL_CHECK_RETURN_MSG(gameObject, E_FAIL, L"CStage::Ready_GameLogic_Layer() failed: CHumanMonster::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::HUMANMONSTER, gameObject),
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(HUMANMONSTER) failed")

        FAILED_CHECK_MSG(
            Engine::AcquirePersistentObject<CMainCamera>(
                OBJTYPE::CAMERA,
                m_GraphicDev,
                layer,
                &gameObject),
            L"Persistent object setup failed");

    m_Layers.insert({ layerType, layer });

    return S_OK;
}



HRESULT CStage::Ready_UI_Layer(LAYERTYPE layerType)
{
    auto layer = Engine::CLayer::Create();
    NULL_CHECK_RETURN(layer, E_FAIL);

    CGameObject* gameObject = nullptr;

    gameObject = CUIPlayerPanel::Create(m_GraphicDev);
    NULL_CHECK_RETURN(gameObject, E_FAIL);

    m_PlayerPanel = dynamic_cast<CUIPlayerPanel*>(gameObject);
    m_PlayerPanel->Set_Player(m_Player);

    if (FAILED(layer->Add_GameObject(OBJTYPE::UI, gameObject)))
        return E_FAIL;
    
    gameObject = CUIMp::Create(m_GraphicDev);
    NULL_CHECK_RETURN(gameObject, E_FAIL);

    if (FAILED(layer->Add_GameObject(OBJTYPE::UI, gameObject)))

        return E_FAIL;

    gameObject = CUICircle::Create(m_GraphicDev);
    NULL_CHECK_RETURN(gameObject, E_FAIL);

    if (FAILED(layer->Add_GameObject(OBJTYPE::UI, gameObject)))
        return E_FAIL;


    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    return S_OK;
}

CStage* CStage::Create(DEVICE graphicDev)
{
    auto stage = new CStage(graphicDev);

    if (FAILED(stage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(stage);
        return nullptr;
    }

    return stage;
}

void CStage::Free()
{
    Engine::CScene::Free();
}
