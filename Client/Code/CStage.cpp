#include "pch.h"
#include "CStage.h"

#include "CLayer.h"
#include "CMainCamera.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CTransform.h"
#include "CTriCol.h"
#include "CState.h"
#include "CManagement.h"
#include "CTestMonster.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CUIPlayerPanel.h"

CStage::CStage(DEVICE graphicDev)
    : Engine::CScene(graphicDev)
{ }

CStage::~CStage()
{ }

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
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Create() returned null")

    Engine::CGameObject* gameObject = nullptr;

    // -----------------------------
    // Player
    // -----------------------------

   /* gameObject = CPlayer::Create(m_GraphicDev);

    m_player = dynamic_cast<CPlayer*>(gameObject);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CStage::Ready_GameLogic_Layer() failed: CPlayer::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::PLAYER, gameObject),
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(PLAYER) failed")

    auto playerTransform =
        dynamic_cast<CTransform*>(gameObject->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM));*/

    // -----------------------------
    // Monster
    // -----------------------------

    // monster
    /*gameObject = CMonster::Create(m_GraphicDev);
    
    if (nullptr == gameObject)
        return E_FAIL;
    
    if (FAILED(layer->Add_GameObject(OBJTYPE::MONSTER, gameObject)))
        return E_FAIL;*/

     //testMonster
  /*  gameObject = CTestMonster::Create(m_GraphicDev);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CStage::Ready_GameLogic_Layer() failed: CMonster::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::BOSS2, gameObject),
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(BOSS2) failed")*/

    // -----------------------------
    // Camera
    // -----------------------------

    //gameObject = CMainCamera::Create(m_GraphicDev);
    //
    //NULL_CHECK_RETURN_MSG(
    //    gameObject,
    //    E_FAIL,
    //    L"CStage::Ready_GameLogic_Layer() failed: CMainCamera::Create() returned null")
    //
    //FAILED_CHECK_MSG(
    //    layer->Add_GameObject(OBJTYPE::CAMERA, gameObject),
    //    L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(CAMERA) failed")
//
//    FAILED_CHECK_MSG(
//        dynamic_cast<CMainCamera*>(gameObject)->Set_CamTarget(playerTransform),
//        L"CStage::Ready_GameLogic_Layer() failed: CCMainCamera::Set_CamTarget() failed")
//
    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(LAYERTYPE layerType)
{
    auto layer = Engine::CLayer::Create();

    NULL_CHECK_RETURN(layer, E_FAIL)

        CGameObject* gameObject = nullptr;

    gameObject = CUIPlayerPanel::Create(m_GraphicDev);

    NULL_CHECK_RETURN(gameObject, E_FAIL)

    m_playerPanel = dynamic_cast<CUIPlayerPanel*>(gameObject);
    m_playerPanel->Set_Player(m_player);

    if (FAILED(layer->Add_GameObject(OBJTYPE::UI, gameObject)))
            return E_FAIL;

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::TRI_COLOR, Engine::CTriCol::Create(m_GraphicDev))))
    //    return E_FAIL;
    //
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::RC_COLOR, Engine::CRcCol::Create(m_GraphicDev))))
    //    return E_FAIL;
    //
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::RC_TEX, Engine::CRcTex::Create(m_GraphicDev))))
    //    return E_FAIL;
    //
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::TRANSFORM, Engine::CTransform::Create(m_GraphicDev))))
    //    return E_FAIL;
    //
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::ANIMATOR, CAnimator::Create(m_GraphicDev))))
    //    return E_FAIL;
    //
    //if (FAILED(CProtoMgr::GetInstance()
    //    ->Ready_Prototype(COMPONENTTYPE::RECT_COLL, CRectCollider::Create(m_GraphicDev))))
    //    return E_FAIL;

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
