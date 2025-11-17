#include "pch.h"
#include "CLoading.h"

#include "CAnimator.h"
#include "CCameraCom.h"
#include "CProtoMgr.h"
#include "CRcCol.h"
#include "CRectCollider.h"
#include "CState.h"
#include "CTexture.h"
#include "CTiled.h"
#include "CTransform.h"
#include "CTriCol.h"

CLoading::CLoading(DEVICE pGraphicDev)
    : m_GraphicDev(pGraphicDev),
      m_Thread(nullptr), m_Crt{}, m_LoadingID(LOADING_END), m_IsFinish(false)
{
    m_GraphicDev->AddRef();
}

CLoading::~CLoading()
{ }

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
    // 스레드 동기화를 위한 임계영역(크리티컬 섹션) 초기화
    InitializeCriticalSection(&m_Crt);

    // 로딩 스레드 생성
    m_Thread = (HANDLE)_beginthreadex(
        nullptr,	        // 보안속성(핸들의 상속 여부, NULL인 경우 상속에서 제외)
        0,		            // 스택 크기 (0 = OS 기본값 사용)
        Thread_Main,		// 쓰레드 함수(__stdcall 호출 규약)
        this,	            // 쓰레드 함수를 이용하여 가공할 데이터 주소
        0,		            // 쓰레드 생성 및 실행을 조절하는 flag
        nullptr);	        // 쓰레드 id 반환

    m_LoadingID = eID;

    return S_OK;
}

_uint CLoading::Loading_ForState()
{
    // TODO 석호: E_FAIL마다 경고 출력도 해야 할듯. 어디서 안되는건지 헷갈림

    auto pProtoMgr = CProtoMgr::GetInstance();

    m_LoadingText = L"Buffer LOADING..........";

    // Buffer : Triangle Color
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::TRI_COLOR, Engine::CTriCol::Create(m_GraphicDev))))
        return E_FAIL;

    // Buffer : Rect Color
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::RC_COLOR, Engine::CRcCol::Create(m_GraphicDev))))
        return E_FAIL;

    // Buffer : Rect UV
    // CLogo 에서 이미 프로토타입 생성중이므로 비활성화
    //if (FAILED(pProtoMgr->Ready_Prototype(
    //    COMPONENTTYPE::RC_TEX, Engine::CRcTex::Create(m_GraphicDev))))
    //    return E_FAIL;

    m_LoadingText = L"Texture LOADING..........";

#pragma region TextureNumber
    // Player
    CTexture* playerTex = CTexture::Create(m_GraphicDev, TEX_NORMAL, L"", 0);
    NULL_CHECK_RETURN(playerTex, E_FAIL);

    if (FAILED(playerTex->Add_Texture(L"PlayerIdle", TEX_NORMAL,
        L"../Bin/Resource/Texture/Test/Player_Idle/Lamb-idle%d.png", 150)))
        return E_FAIL;

    if (FAILED(playerTex->Add_Texture(L"PlayerRunDown", TEX_NORMAL,
        L"../Bin/Resource/Texture/Test/Player_RunDown/Lamb-run-down%d.png", 19)))
        return E_FAIL;

    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::TEX_PLAYER, playerTex)))
        return E_FAIL;

    // Boss2 Test
    CTexture* bossTex = CTexture::Create(m_GraphicDev, TEX_NORMAL, L"", 0);
    NULL_CHECK_RETURN(bossTex, E_FAIL);

    if (FAILED(bossTex->Add_Texture(L"BossIdle", TEX_NORMAL,
        L"../Bin/Resource/Texture/Test/Boss2_Idle/Boss2_Idle%d.png", 400)))
        return E_FAIL;

    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::TEX_MONSTER, bossTex)))
        return E_FAIL;

#pragma endregion

    m_LoadingText = L"Etc LOADING..........";

    // Transform
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::TRANSFORM, Engine::CTransform::Create(m_GraphicDev))))
        return E_FAIL;

    // Animator
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::ANIMATOR, CAnimator::Create(m_GraphicDev))))
        return E_FAIL;

    // Camera
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::CAMERA, Engine::CCameraCom::Create(m_GraphicDev))))
        return E_FAIL;

    // Rect Collider
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::RECT_COLL, CRectCollider::Create(m_GraphicDev))))
        return E_FAIL;

    // State
    if (FAILED(pProtoMgr->Ready_Prototype(
        COMPONENTTYPE::STATE, CState::Create(m_GraphicDev))))
        return E_FAIL;

    // Tile
    if (FAILED(pProtoMgr->Ready_Prototype(COMPONENTTYPE::TEX_TILE,
        CTexture::Create(m_GraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Tile0.png", 1))))
        return E_FAIL;
    

    m_LoadingText = L"LOADING Complete! PRESS ENTER";

    m_IsFinish = true;

    return S_OK;
}

unsigned CLoading::Thread_Main(void* pArg)
{
    auto pLoading = static_cast<CLoading*>(pArg);

    _uint iFlag(0);

    // 임계영역 진입 (다른 스레드의 접근 차단)
    EnterCriticalSection(pLoading->Get_Crt());

    switch (pLoading->Get_LoadingID())
    {
    case LOADING_STAGE:
        iFlag = pLoading->Loading_ForState();
        break;
    case LOADING_BOSS:
        break;
    default:
        break;
    }

    // 임계영역 해제 (다른 스레드 접근 허용)
    LeaveCriticalSection(pLoading->Get_Crt());

    //_endthreadex(0);
    // -> _beginthreadex()로 생성된 스레드는 함수가 return 될 때
    // 내부적으로 _endthreadex() 가 자동 호출되어 정리됨.
    // 따라서 명시적으로 호출할 필요가 없음

    return iFlag;
}

CLoading* CLoading::Create(DEVICE pGraphicDev, LOADINGID eID)
{
    auto pLoading = new CLoading(pGraphicDev);

    if (FAILED(pLoading->Ready_Loading(eID)))
    {
        Safe_Release(pLoading);

        MSG_BOX("Loading Create Failed");
        return nullptr;
    }

    return pLoading;
}

void CLoading::Free()
{
    // 생성한 스레드가 완전히 끝날 때까지 대기
    WaitForSingleObject(m_Thread, INFINITE);

    // 스레드 핸들 닫기 (핸들 리소스 해제)
    CloseHandle(m_Thread);

    // 크리티컬 섹션 삭제 (운영체제 리소스 해제)
    DeleteCriticalSection(&m_Crt);

    Safe_Release(m_GraphicDev);
}
