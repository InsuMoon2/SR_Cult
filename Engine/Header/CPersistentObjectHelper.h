#pragma once
#include "CGameObject.h"
#include "CLayer.h"
#include "CManagement.h"

BEGIN(Engine)
/// @brief Persistent 오브젝트를 조회해서, 존재하지 않을 경우 생성·등록까지 수행하는 함수
/// @tparam T         생성할 오브젝트 타입 (CGameObject 파생 클래스)
/// @param objType    조회 및 캐싱에 사용할 오브젝트 타입
/// @param graphicDev 그래픽 디바이스
/// @param layer      생성 또는 조회된 오브젝트를 추가할 레이어
/// @param outObject  결과로 얻은 오브젝트 포인터를 전달받는 out 파라미터
/// @return 성공 시 S_OK, 생성·등록 실패 또는 잘못된 인자일 경우 E_FAIL
template <typename T>
static HRESULT AcquirePersistentObject(OBJTYPE       objType,
                                       DEVICE        graphicDev,
                                       CLayer*       layer,
                                       CGameObject** outObject)
{
    const auto management = Engine::CManagement::GetInstance();

    FAILED_CHECK(management->Get_PersistentObject(objType, outObject));

    if (nullptr == *outObject)
    {
        *outObject = T::Create(graphicDev);

        NULL_CHECK_RETURN_MSG(
            *outObject,
            E_FAIL,
            L"Obejct create failed");

        FAILED_CHECK_MSG(
            layer->Add_GameObject(objType, *outObject),
            L"Add outObject in layer has failed");

        FAILED_CHECK_MSG(
            management->Cache_PersistentObject(objType, *outObject),
            L"caching outObject in CManagement class has failed");
    }
    else
    {
        (*outObject)->AddRef();

        FAILED_CHECK_MSG(
            layer->Add_GameObject(objType, *outObject),
            L"Add outObject in layer has failed");
    }

    return S_OK;
}

END
