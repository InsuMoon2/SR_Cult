#include "pch.h"
#include "CStage.h"

#include "CAnimator.h"
#include "CLayer.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRcCol.h"
#include "CRcTex.h"
#include "CRectCollider.h"
#include "CTexture.h"
#include "CTransform.h"
#include "CTriCol.h"
#include "CState.h"

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

    if (FAILED(Ready_Camera_Layer(LAYERTYPE::CAMERA)))
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
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, pLayer });          // scene -> map

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* gameObject = nullptr;

    // player
    gameObject = CPlayer::Create(m_GraphicDev);

    if (nullptr == gameObject)
        return E_FAIL;

    if (FAILED(layer->Add_GameObject(OBJTYPE::PLAYER, gameObject)))
        return E_FAIL;

    // monster
    gameObject = CMonster::Create(m_GraphicDev);

    if (nullptr == gameObject)
        return E_FAIL;

    if (FAILED(layer->Add_GameObject(OBJTYPE::MONSTER, gameObject)))
        return E_FAIL;

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_Camera_Layer(LAYERTYPE layerType)
{
    // TODO 석호: 카메라 오브젝트 만든 후 여기에 추가

#pragma region 임시 카메라

    _matrix matView, matProj;

    _vec3 eye = { 0.f, 2.f, -10.f };
    _vec3 at  = { 0.f, 0.f, 1.f };
    _vec3 up  = { 0.f, 1.f, 0.f };

    D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
    D3DXMatrixPerspectiveFovLH(&matProj,
                               D3DXToRadian(60.f),
                               static_cast<float>(WINCX) / WINCY,
                               0.1f,
                               1000.f);

    m_GraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
    m_GraphicDev->SetTransform(D3DTS_VIEW, &matView);

#pragma endregion

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_Layers.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TRI_COLOR, Engine::CTriCol::Create(m_GraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RC_COLOR, Engine::CRcCol::Create(m_GraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RC_TEX, Engine::CRcTex::Create(m_GraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TRANSFORM, Engine::CTransform::Create(m_GraphicDev))))
        return E_FAIL;

    // 빈 Texture를 일단 생성
    CTexture* playerTex = Engine::CTexture::Create(
        m_pGraphicDev,
        TEX_NORMAL,
        L"",      
        0);    // 아무 것도 로드하지 않음
    NULL_CHECK_RETURN(playerTex, E_FAIL);

    // 키값으로 Texture 추가하기
    playerTex->Add_Texture(TEX_NORMAL, L"PlayerIdle",
        L"../Bin/Resource/Texture/Player/Lamb-idle_sheet.png");

    playerTex->Add_Texture(TEX_NORMAL, L"PlayerRunDown",
        L"../Bin/Resource/Texture/Player/Lamb-run-down.png");

    playerTex->Add_Texture(TEX_NORMAL, L"PlayerRun",
        L"../Bin/Resource/Texture/Player/Lamb-run.png");

    playerTex->Add_Texture(TEX_NORMAL, L"PlayerRunUp",
        L"../Bin/Resource/Texture/Player/Lamb-run-up.png");

    playerTex->Add_Texture(TEX_NORMAL, L"BossTest",
        L"../Bin/Resource/Texture/Monster/Boss2/TOWW-Boss2_Idle.png");

    // 세팅 이후 Texture 컴포넌트 프로토타입 생성
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TEXTURE, playerTex)))
        return E_FAIL;

    // Animator
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::ANIMATOR, CAnimator::Create(m_GraphicDev))))
        return E_FAIL;

    // RectCol
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RECT_COLL, CRectCollider::Create(m_GraphicDev))))
        return E_FAIL;

    // State
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::STATE, CState::Create(m_pGraphicDev))))
        return E_FAIL;

    return S_OK;
}

CStage* CStage::Create(DEVICE graphicDev)
{
    auto* stage = new CStage(graphicDev);

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
