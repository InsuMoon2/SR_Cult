#include "CTimerMgr.h"

#include "CTimer.h"

IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{ }

CTimerMgr::~CTimerMgr()
{
    CTimerMgr::Free();
}

_float CTimerMgr::Get_TimeDelta(const wstring& timerTag)
{
    CTimer* timer = Find_Timer(move(timerTag));

    if (nullptr == timer)
        return 0.f;

    return timer->Get_TimeDelta();
}

void CTimerMgr::Set_TimeDelta(const wstring& timerTag)
{
    CTimer* timer = Find_Timer(move(timerTag));

    if (nullptr == timer)
        return;

    timer->Update_Timer();
}

HRESULT CTimerMgr::Ready_Timer(const wstring& timerTag)
{
    CTimer* timer = Find_Timer(timerTag);

    if (timer != nullptr)
        return E_FAIL;

    timer = CTimer::Create();

    if (nullptr == timer)
        return E_FAIL;

    m_Timers.insert({ timerTag, timer });

    return S_OK;
}

CTimer* CTimerMgr::Find_Timer(const wstring& timerTag)
{
    auto iter
        = find_if(m_Timers.begin(), m_Timers.end(), CTag_Finder(move(timerTag)));

    if (iter == m_Timers.end())
        return nullptr;

    return iter->second;
}

void CTimerMgr::Free()
{
    for_each(m_Timers.begin(), m_Timers.end(), CDeleteMap());
    m_Timers.clear();
}
