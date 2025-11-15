#include "pch.h"
#include "CMainApp.h"
#include "CTimerMgr.h"
#include "CManagement.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CStage.h"
#include "CImGuiManager.h"
#include "CCollisionManager.h"

CMainApp::CMainApp() 
    : m_DeviceClass(nullptr), m_GraphicDev(nullptr)
    , m_ManagementClass(CManagement::GetInstance())
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
    if(FAILED(Ready_DefaultSetting(&m_GraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_Scene(m_GraphicDev)))
        return E_FAIL;

    CImGuiManager::GetInstance()->InitImGui(g_hWnd, m_GraphicDev);

    return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
    m_ManagementClass->Update_Scene(fTimeDelta);

    // Imgui
    {
        CImGuiManager::GetInstance()->Update();
        ImGui::Begin("New Windows");
        ImGui::End();
    }

    CCollisionManager::GetInstance()->Update();

    return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
    m_ManagementClass->LateUpdate_Scene(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
    m_DeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));


    m_ManagementClass->Render_Scene(m_GraphicDev);

    CImGuiManager::GetInstance()->Render();
    m_DeviceClass->Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
    if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_DeviceClass)))
        return E_FAIL;
    m_DeviceClass->AddRef();

    (*ppGraphicDev) = m_DeviceClass->Get_GraphicDev();
    (*ppGraphicDev)->AddRef();

    (*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);


    (*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);
    // D3DRS_ZENABLE : Z버퍼에 Z값을 기록, Z 정렬 수행 여부를 묻는 옵션

    (*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    // D3DRS_ZWRITEENABLE : Z버퍼에 Z값을 기록할 지 말 지 여부를 묻는 옵션
    

    return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Engine::CScene* pLogo = CStage::Create(pGraphicDev);
    
    if (nullptr == pLogo)
        return E_FAIL;

    if (FAILED(m_ManagementClass->Set_Scene(pLogo)))
    {
        MSG_BOX("Logo Setting Failed");
        return E_FAIL;
    }

    return S_OK;
}

CMainApp* CMainApp::Create()
{
    CMainApp* pMainApp = new CMainApp;

    if (FAILED(pMainApp->Ready_MainApp()))
    {
        MSG_BOX("CMainApp Create Failed");
        Safe_Release(pMainApp);
        return nullptr;
    }

    return pMainApp;
}

void CMainApp::Free()
{
    Safe_Release(m_GraphicDev);
    Safe_Release(m_DeviceClass);

    CCollisionManager::DestroyInstance();
    CRenderer::DestroyInstance();
    CProtoMgr::DestroyInstance();
    CFrameMgr::DestroyInstance();
    CTimerMgr::DestroyInstance();
    CImGuiManager::DestroyInstance();
    CManagement::DestroyInstance();
    CGraphicDev::DestroyInstance();
}

