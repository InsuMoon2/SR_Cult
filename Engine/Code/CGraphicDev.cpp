#include "CGraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev() : m_pSDK(nullptr), m_pGraphicDev(nullptr)
{
}

CGraphicDev::~CGraphicDev()
{
    Free();
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND hWnd, 
    WINMODE eMode, 
    const _uint& iSizeX, 
    const _uint& iSizeY, 
    CGraphicDev** ppGraphicDev)
{
    // 1. 장치 조사를 위한 객체 생성
    m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
    if (nullptr == m_pSDK)
        return E_FAIL;

    // 2. 장치 성능 조사
    D3DCAPS9       DeviceCaps;
    ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

    if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
        return E_FAIL;

    _ulong  dwFlag(0);

    // vp = 정점 변환 + 조명(명암) 연산

    if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

    else
        dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    D3DPRESENT_PARAMETERS       d3dpp;
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    d3dpp.BackBufferWidth  = iSizeX;
    d3dpp.BackBufferHeight = iSizeY;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount  = 1;

    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;

    // D3DSWAPEFFECT_DISCARD : 스왑 체인
    // D3DSWAPEFFECT_FLIP : 하나의 버퍼를 뒤집어가며 사용
    // D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 유형

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    
    // 창 모드 또는 전체 화면 선택
    d3dpp.Windowed = MODE_WIN;

    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // 3. 그리기 담당 객체 생성

    if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
                                    D3DDEVTYPE_HAL,
                                    hWnd,
                                    dwFlag,
                                    &d3dpp,
                                    &m_pGraphicDev)))
        return E_FAIL;

    *ppGraphicDev = this;

    return S_OK;
}

// 후면 버퍼 동작 원리
// 1. 지우기
// 2. 다음화면에 그릴것 준비
void CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
    m_pGraphicDev->Clear(0,     // 렉트의 개수
                         NULL,  // 렉트의 주소
                        D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 
                        Color,  // 후면 버퍼 텍스처 색상
                        1.f,    // 깊이(z)버퍼의 초기화 값
                        0);     // 스텐실 버퍼 초기화 값

    m_pGraphicDev->BeginScene();
}

void CGraphicDev::Render_End()
{
    m_pGraphicDev->EndScene();
    m_pGraphicDev->Present(NULL, NULL, NULL, NULL);

    // 1, 2 매개 변수 : 렉트의 주소
    // 3 매개 변수 : 핸들(기본 창)
    // 1, 2, 4 매개 변수 : D3DSWAPEFFECT_COPY일 때만 사용 가능
}

void CGraphicDev::Free()
{
    Safe_Release(m_pGraphicDev);
    Safe_Release(m_pSDK);
}
