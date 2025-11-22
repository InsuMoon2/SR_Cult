#include "pch.h"
#include "CStage.h"

#include "CCombatStat.h"
#include "CDropSystem.h"
#include "CHumanMonster.h"
#include "CLayer.h"
#include "CMainCamera.h"
#include "CPersistentObjectHelper.h"
#include "CPlayer.h"
#include "CTestMonster.h"
#include "CTransform.h"
#include "CUICircle.h"
#include "CUIMp.h"
#include "CUIPlayerPanel.h"
#include "CUIWeapon.h"

CStage::CStage(DEVICE graphicDev)
    : Engine::CScene(graphicDev),
      m_PlayerCombatStatCom(nullptr)
{}

CStage::~CStage()
{}

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

    ItemInstance i = { 3001, -1, 0 };
    CDropSystem::GetInstance()->SpawnDrop(m_GraphicDev, i, { 10, 0, 10 });

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

    // 플레이어의 Transform 가져오기
    const auto playerTransform =
        dynamic_cast<CTransform*>(gameObject->Get_Component(
            ID_DYNAMIC,
            COMPONENTTYPE::TRANSFORM));

    m_PlayerCombatStatCom =
        dynamic_cast<CCombatStat*>(gameObject->Get_Component(
            ID_DYNAMIC,
            COMPONENTTYPE::COMBATSTAT));
    //! 여기서 Get_Component를 이용해서, 현재 플레이어를 담고 있는 gameObject 안의
    //! CCombatStat를 정석적인 방법으로 검색해서 m_PlayerCombatStatCom에 저장

    m_PlayerCombatStatCom->AddRef();
    //! 남의 것을 함부로 가져왔으므로 레퍼런스 카운트를 증가시켜서 빌려 쓴다는 것을 명시

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

    // testMonster
    //gameObject = CTestMonster::Create(m_GraphicDev);

    // NULL_CHECK_RETURN_MSG(
    //     gameObject,
    //     E_FAIL,
    //     L"CStage::Ready_GameLogic_Layer() failed: CMonster::Create() returned null");

    // FAILED_CHECK_MSG(
    //     layer->Add_GameObject(OBJTYPE::BOSS2, gameObject),
    //     L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(BOSS2) failed");

    // HumanMonster
    gameObject = CHumanMonster::Create(m_GraphicDev);
    NULL_CHECK_RETURN_MSG(gameObject, E_FAIL, L"CStage::Ready_GameLogic_Layer() failed: CHumanMonster::Create() returned null")

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::HUMANMONSTER, gameObject),
        L"CStage::Ready_GameLogic_Layer() failed: CLayer::Add_GameObject(HUMANMONSTER) failed");

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

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(LAYERTYPE layerType)
{
#pragma region Legecy : 이전에 하던 방식

    //auto layer = Engine::CLayer::Create();
    //NULL_CHECK_RETURN(layer, E_FAIL);
    //
    //CGameObject* gameObject = nullptr;
    //
    //gameObject = CUIPlayerPanel::Create(m_GraphicDev);
    //NULL_CHECK_RETURN(gameObject, E_FAIL);
    //
    //m_playerPanel = dynamic_cast<CUIPlayerPanel*>(gameObject);
    //
    //? 멤버변수 m_playerPanel을 만들어서 여기에 CUIPlayerPanel* gameObject를 담으면 생기는 일
    //! CStage의 m_playerPanel 멤버변수가 우리가 만든 오브젝트의 포인터 주소를 가지고 있게 되고,
    //! CStage가 파괴될 때 그냥 사라져버려 댕글링 포인터가 된다
    //
    //? 이를 안전하게 사용하는 방법?
    //! 사용자가 임의로 만든 포인터 멤버변수는 포인터를 사용만 하고 지우는 건 아무도 하지 않았으므로
    //! 이거에 대해서는 CStage::Free() 에 Safe_Release(m_playerPanel) -> 이런 식으로 넣어서
    //! 직접 지워야 한다
    //! 하지만 이는 어쩔수 없이 직접 이 씬에서 보관해야만 하는 오브젝트에 한해서만 이렇게 하고
    //! 실제로는 아래쪽처럼 멤버변수가 아닌 지역변수로 사용하는 것을 추천한다
    //
    //! 그렇게 해야 CScene -> CLayer -> CGameObject -> CComponent 의 로직 구조가
    //! 각 레이어/오브젝트/컴포넌트를 가지고 있다가 씬 파괴 시 자동으로 없애준다
    //
    //m_playerPanel->Set_Player(m_player);
    //
    //if (FAILED(layer->Add_GameObject(OBJTYPE::UI, gameObject)))
    //    return E_FAIL;
    //
    //m_Layers.insert({ layerType, layer });

#pragma endregion

    auto layer = Engine::CLayer::Create();
    NULL_CHECK_RETURN(layer, E_FAIL);

    CGameObject* gameObject = nullptr;

    gameObject = CUIPlayerPanel::Create(m_GraphicDev);

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::UI, gameObject),
        L"CStage::Ready_UI_Layer() failed: CLayer::Add_GameObject(CUIPlayerPanel) failed");

    CUIPlayerPanel* playerPanel = dynamic_cast<CUIPlayerPanel*>(gameObject);

    // TODO : 예시. Set_Player 라는 오브젝트 가져오기 함수 대신 아래와 같은 컴포넌트 가져오기 함수를 만들어보자
    playerPanel->Set_PlayerCombatStat(m_PlayerCombatStatCom);

    gameObject = CUIMp::Create(m_GraphicDev);

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::UI, gameObject),
        L"CStage::Ready_UI_Layer() failed: CLayer::Add_GameObject(CUIMp) failed");
    //? 석호: OBJTYPE::UI로 통일해서 오브젝트 이름이 겹칠텐데, 그게 정상인가?

    CUIMp* uiMp = dynamic_cast<CUIMp*>(gameObject);
    uiMp->Set_PlayerCombatStat(m_PlayerCombatStatCom);

    gameObject = CUICircle::Create(m_GraphicDev);
     //TODO 석호: 나중에 진짜 만든 후에 활성화 하자!

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::UI, gameObject),
        L"CStage::Ready_UI_Layer() failed: CLayer::Add_GameObject(CUICircle) failed");


    gameObject = CUIWeapon::Create(m_GraphicDev);

    FAILED_CHECK_MSG(
        layer->Add_GameObject(OBJTYPE::UI, gameObject),
        L"CStage::Ready_UI_Layer() failed: CLayer::Add_GameObject(CUIWeapon) failed");

    CUIWeapon* uiweapon = dynamic_cast<CUIWeapon*>(gameObject);

    m_Layers.insert({ layerType, layer });

    //! 일반적으로는 Free() 에서 지우지만, 우리는 사용 후 바로 버리고 싶으므로 여기서 지우도록 하겠다
    Safe_Release(m_PlayerCombatStatCom);

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
