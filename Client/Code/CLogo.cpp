#include "pch.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CPlayer.h"
#include "CMonster.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(LAYERTYPE::Enviroment)))
        return E_FAIL;

    return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CLogo::Render_Scene()
{
   // debug 용
}

HRESULT CLogo::Ready_Environment_Layer(LAYERTYPE eLayerType)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;


    m_mapLayer.insert({ eLayerType , pLayer });    // scene -> map
    
    return S_OK;
}

HRESULT CLogo::Ready_Prototype()
{ 
    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
    //    return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::RcTex, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    // texture
    //if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_LogoTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/sana.jpg", 1))))
    //    return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(COMPONENTTYPE::Texture, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogo* pLogo = new CLogo(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("Logo Create Failed");
        Safe_Release(pLogo);
        return nullptr;

    }

    return pLogo;
}

void CLogo::Free()
{
    Engine::CScene::Free();
}
