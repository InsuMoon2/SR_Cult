#include "pch.h"
#include "CEdit.h"

#include "CLayer.h"
#include "CMainCamera.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CTransform.h"
#include "CTriCol.h"
#include "CState.h"
#include "CManagement.h"
#include "CTestMonster.h"
#include "CTiled.h"

CEdit::CEdit(DEVICE graphicDev)
    : Engine::CScene(graphicDev)
{ }

CEdit::~CEdit()
{ }

HRESULT CEdit::Ready_Scene()
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

_int CEdit::Update_Scene(const _float& timeDelta)
{
    _int exit = Engine::CScene::Update_Scene(timeDelta);

    return exit;
}

void CEdit::LateUpdate_Scene(const _float& timeDelta)
{
    Engine::CScene::LateUpdate_Scene(timeDelta);
}

void CEdit::Render_Scene()
{
    // debug 용
}

HRESULT CEdit::Ready_Environment_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, layer });          // scene -> map

    return S_OK;
}

HRESULT CEdit::Ready_GameLogic_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    NULL_CHECK_RETURN_MSG(
        layer,
        E_FAIL,
        L"CEdit::Ready_GameLogic_Layer() failed: CLayer::Create() returned null")

    Engine::CGameObject* gameObject = nullptr;

    // -----------------------------
    // Player
    // -----------------------------

    gameObject = CPlayer::Create(m_GraphicDev);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CEdit::Ready_GameLogic_Layer() failed: CPlayer::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::PLAYER, gameObject),
        L"CEdit::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(PLAYER) failed")

    auto playerTransform =
        dynamic_cast<CTransform*>(gameObject->Get_Component(ID_DYNAMIC, COMPONENTTYPE::TRANSFORM));

    // -----------------------------
    // Monster
    // -----------------------------

    // monster
    /*gameObject = CMonster::Create(m_GraphicDev);
    
    if (nullptr == gameObject)
        return E_FAIL;
    
    if (FAILED(layer->Add_GameObject(OBJTYPE::MONSTER, gameObject)))
        return E_FAIL;*/

    // testMonster
    gameObject = CTestMonster::Create(m_GraphicDev);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CEdit::Ready_GameLogic_Layer() failed: CMonster::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::BOSS2, gameObject),
        L"CEdit::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(BOSS2) failed")

    // -----------------------------
    // Camera
    // -----------------------------

    gameObject = CMainCamera::Create(m_GraphicDev);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CEdit::Ready_GameLogic_Layer() failed: CMainCamera::Create() returned null");

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::CAMERA, gameObject),
        L"CEdit::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(CAMERA) failed");

    FAILED_CHECK_MSG(
        dynamic_cast<CMainCamera*>(gameObject)->Set_CamTarget(playerTransform),
        L"CEdit::Ready_GameLogic_Layer() failed: CCMainCamera::Set_CamTarget() failed");

    // -----------------------------
    // Tile
    // -----------------------------
    gameObject = CTiled::Create(m_GraphicDev);

    NULL_CHECK_RETURN_MSG(
        gameObject,
        E_FAIL,
        L"CEdit::Ready_GameLogic_Layer() failed: CTiled::Create() returned null");

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::TILE, gameObject),
        L"CEdit::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(Tile) failed");

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CEdit::Ready_UI_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CEdit::Ready_Prototype()
{
    

    return S_OK;
}

CEdit* CEdit::Create(DEVICE graphicDev)
{
    auto stage = new CEdit(graphicDev);

    if (FAILED(stage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(stage);
        return nullptr;
    }

    return stage;
}

void CEdit::Free()
{
    Engine::CScene::Free();
}
