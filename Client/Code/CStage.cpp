#include "pch.h"
#include "CStage.h"
#include "CProtoMgr.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CRectCollider.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(LAYERTYPE::Enviroment)))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(LAYERTYPE::GameLogic)))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(LAYERTYPE::UI)))
        return E_FAIL;

    // 임시

    _matrix     matView, matProj;

    _vec3   vEye = { 0.f, 0.f, -10.f };
    _vec3   vAt = { 0.f, 0.f, 10.f };
    _vec3   vUp = { 0.f, 1.f, 0.f };

    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (float)WINCX / WINCY, 0.1f, 1000.f);

    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
   // debug 용
}

HRESULT CStage::Ready_Environment_Layer(LAYERTYPE eLayerType)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;
  
    m_mapLayer.insert({ eLayerType , pLayer });          // scene -> map

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(LAYERTYPE eLayerType)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    // player
    pGameObject = CPlayer::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(OBJTYPE::Player, pGameObject)))
        return E_FAIL;

    // monster
    pGameObject = CMonster::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(OBJTYPE::Monster, pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ eLayerType , pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(LAYERTYPE eLayerType)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;


    m_mapLayer.insert({ eLayerType , pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::TriCol, Engine::CTriCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::RcCol, Engine::CRcCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::RcTex, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::Transform, Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;

    // texture
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::Texture,
        Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Lamb-idle_sheet.png", 1))))
        return E_FAIL;

    // Animator
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::Animator, CAnimator::Create(m_pGraphicDev))))
        return E_FAIL;

    // RectCol
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::RectCol, CRectCollider::Create(m_pGraphicDev))))
        return E_FAIL;

    return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pStage = new CStage(pGraphicDev);

    if (FAILED(pStage->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pStage);
        return nullptr;

    }

    return pStage;
}

void CStage::Free()
{
    Engine::CScene::Free();
}
