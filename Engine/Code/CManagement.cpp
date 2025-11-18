#include "CManagement.h"

#include "CComponent.h"
#include "CGameObject.h"
#include "CRenderer.h"
#include "CScene.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
    : m_Scene(nullptr),
      m_NextScene(nullptr),
      m_ShouldChangeScene(false),
      m_PlayerObject(nullptr),
      m_CameraObject(nullptr)
{ }

CManagement::~CManagement()
{
    CManagement::Free();
}

CComponent* CManagement::Get_Component(COMPONENTID   componentID,
                                       LAYERTYPE     layerType,
                                       OBJTYPE       objType,
                                       COMPONENTTYPE componentType)
{
    if (nullptr == m_Scene)
        return nullptr;

    return m_Scene->Get_Component(componentID, layerType, objType, componentType);
}

HRESULT CManagement::Cache_PersistentObject(OBJTYPE objType, CGameObject* gameObject)
{
    NULL_CHECK_RETURN(gameObject, E_FAIL)

    CGameObject** target = nullptr;

    switch (objType)
    {
    case OBJTYPE::PLAYER:
        target = &m_PlayerObject;
        break;
    case OBJTYPE::CAMERA:
        target = &m_CameraObject;
        break;
    default:
        return E_FAIL;
    }

    if (*target == gameObject)
        return S_OK;

    Safe_Release(*target);

    *target = gameObject;
    (*target)->AddRef();

    return S_OK;
}

HRESULT CManagement::Set_Scene(CScene* scene)
{
    if (nullptr == scene)
        return E_FAIL;

    Safe_Release(m_Scene);

    m_Scene = scene;

    return S_OK;
}

_int CManagement::Update_Scene(const _float& timeDelta)
{
    if (nullptr == m_Scene)
        return -1;

    return m_Scene->Update_Scene(timeDelta);
}

void CManagement::LateUpdate_Scene(const _float& timeDelta)
{
    m_Scene->LateUpdate_Scene(timeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 graphicDev)
{
    CRenderer::GetInstance()->Render_GameObject(graphicDev);

    m_Scene->Render_Scene();   // 디버깅 출력용
}

HRESULT CManagement::Request_ChangeScene(CScene* nextScene)
{
    if (nullptr == nextScene)
        return E_FAIL;

    if (m_NextScene != nullptr)
        Safe_Release(m_NextScene);

    m_NextScene         = nextScene;
    m_ShouldChangeScene = true;

    return S_OK;
}

void CManagement::Commit_ChangeScene()
{
    if (!m_ShouldChangeScene) return;

    Safe_Release(m_Scene);
    m_Scene             = m_NextScene;
    m_NextScene         = nullptr;
    m_ShouldChangeScene = false;
}

void CManagement::Free()
{
    Safe_Release(m_Scene);
    Safe_Release(m_PlayerObject);
    Safe_Release(m_CameraObject);
}
