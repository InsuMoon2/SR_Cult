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

CStage::CStage(DEVICE graphicDev)
    : Engine::CScene(graphicDev)
{ }

CStage::~CStage()
{ }

HRESULT CStage::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(LAYERTYPE::ENVIRONMENT)))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(LAYERTYPE::GAMELOGIC)))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(LAYERTYPE::CAMERA)))
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

    m_mapLayer.insert({ layerType, pLayer });          // scene -> map

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    // player
    pGameObject = CPlayer::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(OBJTYPE::PLAYER, pGameObject)))
        return E_FAIL;

    // monster
    pGameObject = CMonster::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(OBJTYPE::MONSTER, pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ layerType, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Camera_Layer(LAYERTYPE layerType)
{
    // TODO 석호: 카메라 오브젝트 만든 후 여기에 추가

#pragma region 임시 카메라

    _matrix matView, matProj;

    _vec3 vEye = { 0.f, 0.f, -10.f };
    _vec3 vAt  = { 0.f, 0.f, 10.f };
    _vec3 vUp  = { 0.f, 1.f, 0.f };

    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DXMatrixPerspectiveFovLH(&matProj,
                               D3DXToRadian(60.f),
                               static_cast<float>(WINCX) / WINCY,
                               0.1f,
                               1000.f);

    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

#pragma endregion

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(LAYERTYPE layerType)
{
    Engine::CLayer* layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ layerType, layer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TRI_COLOR, Engine::CTriCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RC_COLOR, Engine::CRcCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RC_TEX, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TRANSFORM, Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;

    // texture
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TEXTURE,Engine::CTexture::Create(
            m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Lamb-idle_sheet.png", 1))))
        return E_FAIL;

    // Animator
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::ANIMATOR, CAnimator::Create(m_pGraphicDev))))
        return E_FAIL;

    // RectCol
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RECT_COLL, CRectCollider::Create(m_pGraphicDev))))
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
