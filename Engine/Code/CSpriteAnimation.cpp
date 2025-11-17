#include "CSpriteAnimation.h"

CSpriteAnimation::CSpriteAnimation()
    : m_MaxFrameX(1),
    m_MaxFrameY(1),
    m_StartFrameX(0),
    m_StartFrameY(0),
    m_FrameX(0),
    m_FrameY(0),
    m_State(ANIMSTATE::STOP),
    m_XAxis(true),
    m_Interval(0.f),
    m_AccTime(0.f)
{
}

CSpriteAnimation::CSpriteAnimation(const CSpriteAnimation& rhs)
{
    m_MaxFrameX = rhs.m_MaxFrameX;
    m_MaxFrameY = rhs.m_MaxFrameY;

    m_StartFrameX = rhs.m_StartFrameX;
    m_StartFrameY = rhs.m_StartFrameY;

    m_FrameX = rhs.m_FrameX;
    m_FrameY = rhs.m_FrameY;
    m_State = rhs.m_State;
    m_XAxis = rhs.m_XAxis;
    m_Interval = rhs.m_Interval;
    m_AccTime = 0.f;
}

CSpriteAnimation::~CSpriteAnimation()
{
    CSpriteAnimation::Free();
}

HRESULT CSpriteAnimation::Ready_SpriteAnim(_uint     maxX,
                            _uint     maxY,
                            _int      startX,
                            _int      startY,
                            _float    interval,
                            bool      xAxis)
{
    m_MaxFrameX = (maxX == 0) ? 1 : maxX;
    m_MaxFrameY = (maxY == 0) ? 1 : maxY;

    // 시작 프레임 기억
    m_StartFrameX = startX;
    m_StartFrameY = startY;

    m_FrameX = startX;
    m_FrameY = startY;

    m_Interval = interval;
    m_XAxis    = xAxis;

    m_State = ANIMSTATE::STOP;

    m_AccTime = 0.f;

    return S_OK;
}

void CSpriteAnimation::Reset()
{
    m_AccTime = 0.f;
    m_FrameX = m_StartFrameX;
    m_FrameY = m_StartFrameY;

}

void CSpriteAnimation::Update(const _float& timeDelta)
{
    if (m_State == ANIMSTATE::STOP) // Stop이면 스프라이트 재생 X
        return;

    m_AccTime += timeDelta;

    // 누적 시간이 Interval을 넘기면, 스프라이트의 다음 프레임으로 이동
    while (m_AccTime >= m_Interval)
    {
        m_AccTime -= m_Interval;

        if (m_XAxis) // 가로 재생
        {
            if (m_State == ANIMSTATE::LOOP) // 계속 반복
            {
                m_FrameX++;

                if (m_FrameX >= m_MaxFrameX)
                    m_FrameX = 0;
            }

            else if (m_State == ANIMSTATE::ONCE) // 한번만 재생 후 Stop
            {
                m_FrameX++;

                if (m_FrameX >= m_MaxFrameX)
                {
                    m_FrameX = m_MaxFrameX - 1;
                    m_State  = ANIMSTATE::STOP;
                }
            }

            else if (m_State == ANIMSTATE::REVERSE_LOOP)    // 반대로 반복재생
            {
                m_FrameX--;

                if (m_FrameX < 0)
                    m_FrameX = m_MaxFrameX - 1;
            }

            else if (m_State == ANIMSTATE::REVERSE_ONCE)    // 반대로 한번만 재생
            {
                m_FrameX--;

                if (m_FrameX < 0)
                {
                    m_FrameX = 0;
                    m_State  = ANIMSTATE::STOP;
                }
            }
        }

        else // 세로 재생
        {
            if (m_State == ANIMSTATE::LOOP)
            {
                m_FrameY++;

                if (m_FrameY >= m_MaxFrameY)
                    m_FrameY = 0;
            }
            else if (m_State == ANIMSTATE::ONCE)
            {
                m_FrameY++;

                if (m_FrameY >= m_MaxFrameY)
                {
                    m_FrameY = m_MaxFrameY - 1;
                    m_State  = ANIMSTATE::STOP;
                }
            }
            else if (m_State == ANIMSTATE::REVERSE_LOOP)
            {
                m_FrameY--;

                if (m_FrameY < 0)
                    m_FrameY = m_MaxFrameY - 1;
            }
            else if (m_State == ANIMSTATE::REVERSE_ONCE)
            {
                m_FrameY--;

                if (m_FrameY < 0)
                {
                    m_FrameY = 0;
                    m_State  = ANIMSTATE::STOP;
                }
            }
        }
    }
}

void CSpriteAnimation::Get_UV(_float& u0, _float& v0, _float& u1, _float& v1) const
{
    if (m_MaxFrameX > 1)
    {
        u0 = (m_FrameX) / static_cast<float>(m_MaxFrameX);
        u1 = (m_FrameX + 1.f) / static_cast<float>(m_MaxFrameX);
    }

    else
    {
        u0 = 0.f;
        u1 = 1.f;
    }

    if (m_MaxFrameY > 1)
    {
        v0 = (m_FrameY) / static_cast<float>(m_MaxFrameY);
        v1 = (m_FrameY + 1.f) / static_cast<float>(m_MaxFrameY);
    }
    else
    {
        v0 = 0.f;
        v1 = 1.f;
    }
}

CSpriteAnimation* CSpriteAnimation::Create(_uint maxX, _uint maxY, _int startX, _int startY, _float interval, _bool xAxis)
{
    auto sprite = new CSpriteAnimation();

    if (FAILED(sprite->Ready_SpriteAnim(maxX, maxY, startX, startY, interval, xAxis)))
    {
        Safe_Release(sprite);
        MSG_BOX("Sprite Create Failed");
        return nullptr;
    }

    return sprite;
}

void CSpriteAnimation::Free()
{ }
