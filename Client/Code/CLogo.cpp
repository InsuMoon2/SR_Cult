#include "pch.h"
#include "CLogo.h"

#include "CFontMgr.h"
#include "CLayer.h"
#include "CMonster.h"
#include "CProtoMgr.h"
#include "CRcTex.h"

CLogo::CLogo(DEVICE graphicDev)
    : Engine::CScene(graphicDev)
{ }

CLogo::~CLogo()
{ }

HRESULT CLogo::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(LAYERTYPE::ENVIRONMENT)))
        return E_FAIL;

    return S_OK;
}

_int CLogo::Update_Scene(const _float& timeDelta)
{
    _int exit = Engine::CScene::Update_Scene(timeDelta);

    return exit;
}

void CLogo::LateUpdate_Scene(const _float& timeDelta)
{
    Engine::CScene::LateUpdate_Scene(timeDelta);
}

void CLogo::Render_Scene()
{
    // debug 용

    _vec2 pos{ 100.f, 100.f };

    CFontMgr::GetInstance()->Render_Font(L"Font_Default",
                                         L"폰트 출력 테스트",
                                         &pos,
                                         D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Environment_Layer(LAYERTYPE layerType)
{
    auto layer = Engine::CLayer::Create();

    if (nullptr == layer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ layerType, layer });

    return S_OK;
}

HRESULT CLogo::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::RC_TEX, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()
        ->Ready_Prototype(COMPONENTTYPE::TEXTURE, Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    return S_OK;
}

CLogo* CLogo::Create(DEVICE graphicDev)
{
    auto logo = new CLogo(graphicDev);

    if (FAILED(logo->Ready_Scene()))
    {
        MSG_BOX("Logo Create Failed");
        Safe_Release(logo);
        return nullptr;
    }

    return logo;
}

void CLogo::Free()
{
    Engine::CScene::Free();
}
